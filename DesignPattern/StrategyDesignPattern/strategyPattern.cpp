#include<bits/stdc++.h>
using namespace std;
 
class TalkableRobot{
      public:
      virtual void talk()=0;
      virtual ~TalkableRobot(){};
};

class NormalTalk :public TalkableRobot{
     public:
     void talk() override{
          cout<<"Normal Walk"<<endl;
     }
};

class NonTalk:public TalkableRobot{
      public:
      void talk()override{
          cout<<"cannot Walk"<<endl;
      }
};

class WalkableRobot{
      public:
      virtual void walk()=0;
      virtual ~WalkableRobot(){};
};

class NormalWalk:public WalkableRobot{
      public:
      void walk() override{
          cout<<" Normal Walk"<<endl;
      }
};
class NonWalk : public WalkableRobot{
       public:
      void walk() override{
          cout<<" cannot Walk"<<endl;
      }
};

class FlyableRobot{
      public:
      virtual void fly()=0;
      virtual ~FlyableRobot(){}
};

class NormalFly:public FlyableRobot{
      public:
      void fly() override{
          cout<<" Normal Fly"<<endl;
      }
};

class NonFly:public FlyableRobot{
       public:
      void fly() override{
          cout<<" cannot Fly"<<endl;
      }
};

class Robot{
      WalkableRobot*walkbehaviour;
      TalkableRobot*talkbehaviour;
      FlyableRobot*flyBehavior;
      public:
      Robot(WalkableRobot*w,TalkableRobot*t,FlyableRobot*f){
          this->walkbehaviour=w;
          this->talkbehaviour=t;
          this->flyBehavior=f;
      }

      void walk(){
          walkbehaviour->walk();
      }
      void talk(){
         talkbehaviour->talk();
      }
      void fly(){
         flyBehavior->fly();
      }

      virtual void Projection()=0;
};

class CampanionRobot:public Robot{
      public:
      CampanionRobot(WalkableRobot*w,TalkableRobot*t,FlyableRobot*f):Robot(w,t,f){}
      void Projection() override{
          cout<<" Display Campanion Feature"<<endl;
      }

};
class WorkerRobot:public Robot{
      public:
      WorkerRobot(WalkableRobot*w,TalkableRobot*t,FlyableRobot*f):Robot(w,t,f){}
      void Projection() override{
          cout<<" Display Worker Feature"<<endl;
      }
};
int main(){

    Robot *robot1=new CampanionRobot(new NonWalk(),new NormalTalk(),new NonFly());
    robot1->walk();
    robot1->talk();
    robot1->fly();
   robot1->Projection();

   Robot *robot2=new WorkerRobot(new NonWalk,new NonTalk,new NormalFly());
   robot2->walk();
    robot2->talk();
    robot2->fly();
   robot2->Projection();

   return 0;
}