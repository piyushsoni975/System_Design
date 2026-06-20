#include<bits/stdc++.h>
#include<mutex>
#include <memory>
using namespace std;

enum class MachineState{
       IDEL,
       MONEY_INSERTED,
       DISPENSING
};

class Product {

      private:
          string productId;
          string name;
          double price;
          string category;

     public:
          
         Product(){}
         
         Product(string id,string name, double price, string category):
         productId(id),name(name),price(price),category(category){}

         string getName(){
              return name;
         }

         double getPrice(){
              return price;
         }

         string getCategory(){
              return category;
         }

};

class Slot{

       private:
          string slotId;
          shared_ptr<Product> product;
          int quantity;
          int maxCapacity;

       public:
          
       Slot(){}

       Slot(string id,shared_ptr<Product>product,int quantity,int maxCapacity):
       slotId(id),product(product),quantity(quantity),maxCapacity(maxCapacity) {}

       bool isAvailable(){
          return quantity>0;
       }

       shared_ptr<Product> dispense(){
          if(quantity<=0){
              return NULL;
          }
          quantity--;
          return product;
       }

       void restock(int qty){
            quantity=min(maxCapacity,quantity+qty);
       }

       int getQuatity(){
         return quantity;
       }

       string getSlotId(){
          return slotId;
       }

       shared_ptr<Product> getProduct(){
          return product;
       }
};

class Inventory{
     private:
         unordered_map<string,shared_ptr<Slot>>slots;
         
         Inventory(){}

    public:
        Inventory(Inventory&)=delete;

        Inventory& operator=(
            Inventory&
        )=delete;

        static Inventory&
        getInstance(){
              static Inventory instance;

              return instance;
        }

        void addSlot(const string& id,shared_ptr<Slot>slot){
                slots[id]=slot;
        }

        shared_ptr<Slot>getSlot(const string &id){
               if(slots.find(id)==slots.end()){
                 return NULL;
               }

               return slots[id];
        }
        
        bool isAvailable(const string &id){
              auto slot=getSlot(id);

              return slot && slot->isAvailable();
        }

        void restock(const string &id,int qty){
              auto slot=getSlot(id);
              if(slot){
                  slot->restock(qty);
              }
        }

        void displayInventory(){
                   cout
        << "\n----- INVENTORY -----\n";

        for(auto it:slots){
              auto slot=it.second;

              cout<<"Slot: "<<slot->getSlotId()<<" Product: "<<
              slot->getProduct()->getName()<<" Price: "<<slot->getProduct()->getPrice()<<
              " Quantity: "<<slot->getQuatity()<<endl;   
              
        }
        cout<<endl;
        }


};

class PaymentStrategy{
     public:
        
        virtual bool processPayment(double amount)=0;

        virtual double returnChange()=0;

        virtual string getType()=0;

        virtual ~PaymentStrategy(){}
};

class CashPayment: public PaymentStrategy{
     private:
     double inserted;

     public:
      CashPayment(double amount) : inserted(amount){}

      bool processPayment(double amount)override{
                return inserted>=amount;
      }

      void deduct(double amount){
          inserted-=amount;
      }

      double returnChange() override{
            return inserted;
      }

      string getType() override{
          return "Cash";
      }
};

class CardPayment:public PaymentStrategy{
      private:
      string cardToken;

      public:

      CardPayment(
        string token
    ) : cardToken(token) {}

    bool processPayment(double ) override{
              return true;
    }

    double returnChange() override{
         return 0;
    }

    string getType() override{
          return "Card";
    }


};

class Display{
     public:
      void showWelcome() {

        cout
        << "Welcome to Vending Machine\n";
    }

    void showBalance(double amount){
          cout<<"Current Balance: "<<amount<<endl;
    }

    void showError(const string &msg){
          cout<<"[ERROR] "<<msg<<endl;
    }

    void showMessage(const string &msg){
          cout<<msg<<endl;
    }


};

class Transaction {
      private:
        string taxId;
        double amountPaid;
        double change;
        string status;

     public:

       Transaction(string taxid,double amountPaid,double change,string status):
       taxId(taxid),amountPaid(amountPaid),change(change),status(status){}

       void print(){
            cout
        << "\n------ RECEIPT ------\n";

        cout<<" Transaction Id: "<<taxId<<endl;
        cout<<" Amount Paid: "<<amountPaid<<endl;
        cout<<" Change: "<<change<<endl;
        cout<<" Status: "<<status<<endl;

        cout
        << "---------------------\n";
       }
};

class VendingMachine {
       private:
       MachineState state;

       unique_ptr<PaymentStrategy>paymentStrategy;

       Display display;

       mutex mtx;

       VendingMachine(){
         state=MachineState::IDEL;
       }

    public:
       VendingMachine(VendingMachine&) = delete;
       VendingMachine& operator=(VendingMachine&) = delete;

       static VendingMachine& getInstance(){
           static VendingMachine instance;
           return instance;
       }

       void insertPayment(unique_ptr<PaymentStrategy>strategy){
           lock_guard<mutex>lock(mtx);
           paymentStrategy=move(strategy);

           state=MachineState::MONEY_INSERTED;

           string msg="Payment Accepted";
           display.showMessage(msg);


       }

       void selectProduct(const string &slotId){
         lock_guard<mutex>lock(mtx);

         auto slot=Inventory::getInstance().getSlot(slotId);

         if(slot==NULL){
              string error= "Invalid Slot";
              display.showError(error);
              return;
         }
         if(!slot->isAvailable()){
            string error="Out Of Stock";
              display.showError(error);
              return;
         }

         auto product=slot->getProduct();
         double price=product->getPrice();
         
         if(!paymentStrategy->processPayment(price)){
              string error="Insufficient Funds";
              display.showError(error);
              return;
         }

         state=MachineState::DISPENSING;
         slot->dispense();

         double change=0;
         if(auto cash=dynamic_cast<CashPayment*>(paymentStrategy.get())){
              cash->deduct(price);
              change=cash->returnChange();
         }

         cout<< "\nDispensing Product: "<<product->getName()<<endl;

         string id="TXN001",status="SUCCESS";

         Transaction txn(id,price,change,status);

         txn.print();
         
         state=MachineState::IDEL;
         
       }
       void showInventory(){
          Inventory::getInstance().displayInventory();
       }
};

int main(){
       auto coke=make_shared<Product>("p1","Coke",30,"Drink");
        auto pepsi=make_shared<Product>("p2","Pepsi",25,"Drink");
         auto chips=make_shared<Product>("p3","Chips",20,"Snaks");

         Inventory::getInstance().addSlot("A1",make_shared<Slot>("A1",coke,5,10));

         Inventory::getInstance().addSlot("A2",make_shared<Slot>("A2",pepsi,3,10));
         Inventory::getInstance().addSlot("A3",make_shared<Slot>("A3",chips,7,10));

         auto &vm=VendingMachine::getInstance();

         vm.showInventory();
         vm.insertPayment(make_unique<CashPayment>(50));
         vm.selectProduct("A1");

         vm.showInventory();

         return 0;
}


