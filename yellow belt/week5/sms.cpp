#include <iostream>
using namespace std;

void SendSms(const string& number, const string& message) {
    cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string& email, const string& message) {
    cout << "Send '" << message << "' to e-mail "  << email << endl;
}

class INotifier {
public:
    virtual void Notify(const string& message) = 0;
};

class SmsNotifier : public INotifier {
public:
    SmsNotifier(const string& new_number) {
        number = new_number;
    }
    void Notify(const string& message) override {
        SendSms(number, message);
    }
protected:
    string number;
};

class EmailNotifier : public INotifier {
public:
    EmailNotifier(const string& new_email) {
        email = new_email;
    }
    void Notify(const string& message) override {
        SendEmail(email, message);
    }
protected:
    string email;
};

void Notify(INotifier& notifier, const string& message) {
    notifier.Notify(message);
}

int main() {
    SmsNotifier sms{"+7-495-777-77-77"};
    EmailNotifier email{"na-derevnyu@dedushke.ru"};

    Notify(sms, "I have White belt in C++");
    Notify(email, "And want a Yellow one");
    return 0;
}
