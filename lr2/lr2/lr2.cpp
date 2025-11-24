#include <iostream>
#include <string>

class SocialNetworkConnector {
public:
    virtual ~SocialNetworkConnector() = default;
    virtual bool logIn() = 0;
    virtual void postMessage(const std::string& message) = 0;
    virtual void logOut() = 0;
};

class FacebookConnector : public SocialNetworkConnector {
    std::string login;
    std::string password;
public:
    FacebookConnector(const std::string& l, const std::string& p)
        : login(l), password(p) {
    }

    bool logIn() override {
        std::cout << "Logged in to Facebook\n";
        return true;
    }

    void postMessage(const std::string& message) override {
        std::cout << "Facebook: " << message << "\n";
    }

    void logOut() override {
        std::cout << "Logged out from Facebook\n";
    }
};

class LinkedInConnector : public SocialNetworkConnector {
    std::string email;
    std::string password;
public:
    LinkedInConnector(const std::string& e, const std::string& p)
        : email(e), password(p) {
    }

    bool logIn() override {
        std::cout << "Logged in to LinkedIn\n";
        return true;
    }

    void postMessage(const std::string& message) override {
        std::cout << "LinkedIn: " << message << "\n";
    }

    void logOut() override {
        std::cout << "Logged out from LinkedIn\n";
    }
};

class SocialNetworkFactory {
public:
    virtual ~SocialNetworkFactory() = default;
    virtual SocialNetworkConnector* createConnector() const = 0;

    void publish(const std::string& message) {
        SocialNetworkConnector* connector = createConnector();
        connector->logIn();
        connector->postMessage(message);
        connector->logOut();
        delete connector;
    }
};

class FacebookFactory : public SocialNetworkFactory {
    std::string login;
    std::string password;
public:
    FacebookFactory(const std::string& l, const std::string& p)
        : login(l), password(p) {
    }

    SocialNetworkConnector* createConnector() const override {
        return new FacebookConnector(login, password);
    }
};

class LinkedInFactory : public SocialNetworkFactory {
    std::string email;
    std::string password;
public:
    LinkedInFactory(const std::string& e, const std::string& p)
        : email(e), password(p) {
    }

    SocialNetworkConnector* createConnector() const override {
        return new LinkedInConnector(email, password);
    }
};

int main() {
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "ukr");
    SocialNetworkFactory* fbFactory = new FacebookFactory("user123", "pass123");
    fbFactory->publish("Hello from Factory Method – Facebook!");
    delete fbFactory;

    std::cout << "------------------------\n";

    SocialNetworkFactory* liFactory = new LinkedInFactory("user@company.com", "secret");
    liFactory->publish("Professional post – LinkedIn!");
    delete liFactory;

    return 0;
}