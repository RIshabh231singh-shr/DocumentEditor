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