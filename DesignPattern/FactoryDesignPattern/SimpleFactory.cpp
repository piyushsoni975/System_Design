#include<bits/stdc++.h>
using namespace std;

class Burger{
    public:
      virtual void prepare()=0;
      virtual ~Burger(){};
};

class BasicBurger:public Burger{
        public:
        void prepare() override{
              cout<<"Preparing Basic Burger with bun, patty, and ketchup!"<<endl;
        }
};

class StanderedBurger:public Burger{
      public:
      void prepare() override{
          cout<<"Preparing Standard Burger with bun, patty, cheese, and lettuce!"<<endl;
      }
};

class PremiumBurger : public Burger{
       public:
       void prepare() override{
          cout<<"Preparing Premium Burger with gourmet bun, premium patty, cheese, lettuce, and secret sauce!"<<endl;
       }
};

class BurgerFactory{
      public:
        Burger *createBurger(string &s){
               if(s=="basic"){
                   return new BasicBurger();
               }
               else if(s=="standard"){
                  return new StanderedBurger();
               }
               else if(s=="premium"){
                  return new PremiumBurger();
               }
               else{
                  cout<<"Invalid Type !"<<endl;
                  return NULL;
               }
        }
};
int main(){
       string type="premium";

       BurgerFactory*myBurgerFactory=new BurgerFactory();
       Burger *burger=myBurgerFactory->createBurger(type);
       burger->prepare();

       return 0;
}