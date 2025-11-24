#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Notification {
public:
    virtual ~Notification() = default;
    virtual void send(const std::string& title, const std::string& message) = 0;
};

class EmailNotification : public Notification {
private:
    std::string adminEmail;
public:
    explicit EmailNotification(const std::string& adminEmail) : adminEmail(adminEmail) {}
    void send(const std::string& title, const std::string& message) override {
        std::cout << "Sent email with title '" << title
            << "' to '" << adminEmail
            << "' that says '" << message << "'.\n";
    }
};

class SlackAPI {
private:
    std::string login;
    std::string apiKey;
public:
    SlackAPI(const std::string& login, const std::string& apiKey)
        : login(login), apiKey(apiKey) {
    }
    void sendMessage(const std::string& chatId, const std::string& message) const {
        std::cout << "Sent Slack message to chat " << chatId
            << ": " << message << "\n";
    }
};

class SmsProvider {
public:
    void sendSms(const std::string& phone, const std::string& sender, const std::string& text) const {
        std::cout << "Sent SMS from '" << sender
            << "' to '" << phone << "': " << text << "\n";
    }
};

class SlackNotificationAdapter : public Notification {
private:
    SlackAPI slack;
    std::string chatId;
public:
    SlackNotificationAdapter(const std::string& login,
        const std::string& apiKey,
        const std::string& chatId)
        : slack(login, apiKey), chatId(chatId) {
    }
    void send(const std::string& title, const std::string& message) override {
        std::string fullMessage = "[" + title + "] " + message;
        slack.sendMessage(chatId, fullMessage);
    }
};

class SmsNotificationAdapter : public Notification {
private:
    SmsProvider smsProvider;
    std::string phone;
    std::string sender;
public:
    SmsNotificationAdapter(const std::string& phone,
        const std::string& sender)
        : phone(phone), sender(sender) {
    }
    void send(const std::string& title, const std::string& message) override {
        std::string fullMessage = title + ": " + message;
        smsProvider.sendSms(phone, sender, fullMessage);
    }
};

int main() {
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "ukr");
    std::vector<std::unique_ptr<Notification>> notifications;

    notifications.push_back(std::make_unique<EmailNotification>("admin@example.com"));

    notifications.push_back(std::make_unique<SlackNotificationAdapter>(
        "my_login", "my_api_key_12345", "#general"));

    notifications.push_back(std::make_unique<SmsNotificationAdapter>(
        "+380991234567", "CompanyBot"));

    std::string title = "Системне попередження";
    std::string message = "Температура сервера перевищила 80°C!";

    for (const auto& n : notifications) {
        n->send(title, message);
    }

    return 0;
}