#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

class INotification{
    public:
    virtual string getContent() const = 0;
};

class SimpleNotification : public INotification{
    private:
    string text;

    public:
    SimpleNotification(string const &text){
        this->text = text;
    }
    string getContent() const override{
        return text;
    }
};

//decorator pattern 
class INotificationDecorator : public INotification{
    protected:
    INotification* notification;
    //jo has-a relationship ke liye liya tha usko constructor se initialise kr diya
    public:
    INotificationDecorator(INotification* n){
        notification = n;
    }
    virtual ~INotificationDecorator() {
        delete notification;
    }
};

class TimestampDecorator : public INotificationDecorator{
    public:
    TimestampDecorator(INotification* n) : INotificationDecorator(n){};

    string getContent() const override {
        return "[2025-04-13 14:22:00] " + notification->getContent();
    }
};

class SignatureStampDecorator : public INotificationDecorator{
    private:
    string signature;
    public:
    SignatureStampDecorator(INotification* n,string signature) : INotificationDecorator(n){
        this->signature = signature;
    };

    string getContent() const override {
        return notification->getContent() + "\n-- " + signature + "\n\n";
    }
};



//Observer Pattern

class Iobserver{
    public : 
    virtual void update() = 0;
    virtual ~Iobserver(){};
};
class Iobservable{
    public :
    virtual void addObserver(Iobserver* Observer) = 0;    
    virtual void removeObserver(Iobserver* Observer) =0;
    virtual void notifyObserver() = 0;
};

//concrete observable

class NotificationObservable : public Iobservable{
    private :
    vector<Iobserver*>Observers;
    INotification* currentNotification;
    public:
    NotificationObservable(){
        currentNotification = nullptr;
    }
    void addObserver(Iobserver* obs) override{
        auto it = find(Observers.begin(),Observers.end(),obs);
        if(it == Observers.end()){
            Observers.push_back(obs);
            return;
        }
    }
    void removeObserver(Iobserver* obs) override{
        auto it = find(Observers.begin(),Observers.end(),obs);
        if(it != Observers.end()){
            Observers.erase(it);
        }
    }

    void notifyObserver() override{
        for(auto it  : Observers){
            it->update();  //ye it observer ka pointer datatype ka hoga kyuki lidt me whi hai
        }
    }
    //Making setters and getters
    void setNotification(INotification* notification){
        if(currentNotification != nullptr){
            delete currentNotification;
        } //check kiya kiphle se koi to notification nhi hai agar hai to usko delete kr do
        currentNotification = notification; //jo naya notification aaya hai usko current me daal do
        notifyObserver(); //observer ko notify
    }
    INotification* getNotification(){
        if(currentNotification) return currentNotification;
    }

    string getNotificationContent(){
        if(currentNotification) return currentNotification->getContent();
    }
    ~NotificationObservable() {
        if (currentNotification != NULL) {
            delete currentNotification;
        }
    }
};

//Notification service
//works as manager and is singleton
class NotificationService{
    private : 
    NotificationObservable* Observable;
    static NotificationService* instance ;
    vector<INotification*> notifications;
    //private constructor
    NotificationService(){
        Observable = new NotificationObservable();
    }
    public:
    static NotificationService* getInstance(){
        if(instance == nullptr){
            instance = new NotificationService();
        }
        return instance;
    }
    NotificationObservable* getObservable(){
        return Observable;
    }
    
    void sendNotification(INotification* notification) {
        notifications.push_back(notification);
        Observable->setNotification(notification);
    }

    ~NotificationService() {
        delete Observable;
    }
};
NotificationService* NotificationService :: instance = nullptr;

//concrete Observers

class Logger : public Iobserver{
    private : 
    NotificationObservable* notificationObservable;
    public:
    Logger() {
       this->notificationObservable = NotificationService::getInstance()->getObservable();
       notificationObservable->addObserver(this); //this is pointer to current object Logger
    }
    Logger(NotificationObservable* notificationObservable){
        this->notificationObservable = notificationObservable;
        notificationObservable->addObserver(this);
    }
    void update(){
         cout << "Logging New Notification : \n" << notificationObservable->getNotificationContent();
    }
};
