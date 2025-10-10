#include <bits/stdc++.h>
using namespace std;

// Abstraction for document element
class DocumentElement{
       public:
       virtual string render()=0;
};

class TextElement:public DocumentElement{
       private:
       string text;
       public:
         TextElement(string text){
              this->text=text;
         }
         string render()override{
              return text;
         }
};

// Implement for Image element
class ImageElement:public DocumentElement{
       private:
        string text;
        public:
        ImageElement(string text){
              this->text=text;
        }

        string render()override{
              return "[Image"+text+"]";
        }
};

// give a line breake
class NewLineElement:public DocumentElement{
        public:
         string render() override{
              return "\n";
         }
};

// give a tab space 
class TabSpace :public DocumentElement{
      public:
      string render() override{
           return "\t";
      }
};

// Document class is responsible for holding the collection of element
class Document{
       private:
       vector<DocumentElement*>documentElement;
       public:
       void addElement(DocumentElement*element){
           documentElement.push_back(element);
       }
       //render the document by concating the render o/p of all element
       string render(){
         string ans;
           for(auto it:documentElement){
               ans+=it->render();
           }
           return ans;
       }
};

//persistance abstraction
class Persistence{
    public:
    virtual void save(string text)=0;
};

//File storage implementation of persistence
class FileStorage:public Persistence{
       public:
       void save(string text) override{
           ofstream outFile("document.txt");
           if(outFile){
              outFile<<text;
              outFile.close();
              cout<<"Document saved to document.txt"<<endl;
           }
           else{
               cout<<"error"<<endl;
           }
       }
};
// placeholder DB Storage implementation

class DBstorage:public Persistence{
       public:
        void save(string text) override{
               
        }
};

//Document Editor class managing the client interaction
class DocumentEditor{
       private:
       Document*document;
       Persistence*storage;
       string renderedDocument;

       public:

       DocumentEditor(Document* document,Persistence* storage){
        this->document=document;
        this->storage=storage;
       }

       void addText(string text){
            document->addElement(new TextElement(text));
       }
       void addImage(string text){
          document->addElement(new ImageElement(text));
       }
       void addNewLine(){
          document->addElement(new NewLineElement());
       }
       void addTabSpace(){
            document->addElement(new TabSpace());
       }
       string renderDocument(){
          if(renderedDocument.empty()){
              renderedDocument=document->render();
          }
          return renderedDocument;
       }
       void saveDocument(){
          storage->save(renderDocument());
       }
};

//client example
int main(){
         Document*document=new Document();
         Persistence*persitance=new FileStorage();
         DocumentEditor*editor=new DocumentEditor(document,persitance);

         editor->addText("Hello world !");
         editor->addNewLine();
         editor->addText("This is real world Document");
         editor->addNewLine();
         editor->addTabSpace();
         editor->addText("Intended Text after Tabsapce ");
         editor->addNewLine();
         editor->addImage("picture.jpg");

         cout<<editor->renderDocument()<<endl;

         editor->saveDocument();

         return 0;
}
