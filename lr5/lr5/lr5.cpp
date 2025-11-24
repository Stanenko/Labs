#include <iostream>
#include <string>

struct Product {
    int id;
    std::string name;
    std::string description;
    std::string imageUrl;

    Product(int i, std::string n, std::string d, std::string img)
        : id(i), name(std::move(n)), description(std::move(d)), imageUrl(std::move(img)) {
    }
};

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual std::string beginDocument() const { return ""; }
    virtual std::string endDocument() const { return ""; }

    virtual std::string renderTitle(const std::string& title) const = 0;
    virtual std::string renderContent(const std::string& content) const = 0;
    virtual std::string renderProduct(const Product& p) const = 0;
};

class HTMLRenderer : public Renderer {
public:
    std::string beginDocument() const override { return "<html><body>\n"; }
    std::string endDocument() const override { return "\n</body></html>"; }

    std::string renderTitle(const std::string& title) const override {
        return "<h1>" + title + "</h1>\n";
    }
    std::string renderContent(const std::string& content) const override {
        return "<p>" + content + "</p>\n";
    }
    std::string renderProduct(const Product& p) const override {
        return "<div class=\"product\">\n"
            "  <h2>" + p.name + "</h2>\n"
            "  <img src=\"" + p.imageUrl + "\" alt=\"" + p.name + "\" />\n"
            "  <p>ID: " + std::to_string(p.id) + "</p>\n"
            "  <p>" + p.description + "</p>\n"
            "</div>";
    }
};

class JsonRenderer : public Renderer {
public:
    std::string renderTitle(const std::string& title) const override {
        return "\"title\": \"" + title + "\"";
    }
    std::string renderContent(const std::string& content) const override {
        return "\"content\": \"" + content + "\"";
    }
    std::string renderProduct(const Product& p) const override {
        return "{\n"
            "  \"id\": " + std::to_string(p.id) + ",\n"
            "  \"name\": \"" + p.name + "\",\n"
            "  \"description\": \"" + p.description + "\",\n"
            "  \"imageUrl\": \"" + p.imageUrl + "\"\n"
            "}";
    }
    std::string beginDocument() const override { return "{\n  "; }
    std::string endDocument() const override { return "\n}"; }
};

class XmlRenderer : public Renderer {
public:
    std::string beginDocument() const override { return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<page>\n"; }
    std::string endDocument() const override { return "\n</page>"; }

    std::string renderTitle(const std::string& title) const override {
        return "  <title>" + title + "</title>\n";
    }
    std::string renderContent(const std::string& content) const override {
        return "  <content>" + content + "</content>\n";
    }
    std::string renderProduct(const Product& p) const override {
        return "  <product>\n"
            "    <id>" + std::to_string(p.id) + "</id>\n"
            "    <name>" + p.name + "</name>\n"
            "    <description>" + p.description + "</description>\n"
            "    <image>" + p.imageUrl + "</image>\n"
            "  </product>";
    }
};

class Page {
protected:
    Renderer* renderer;
public:
    explicit Page(Renderer* r) : renderer(r) {}
    virtual ~Page() = default;
    virtual void view() const = 0;
};

class SimplePage : public Page {
    std::string title;
    std::string content;
public:
    SimplePage(Renderer* r, std::string t, std::string c)
        : Page(r), title(std::move(t)), content(std::move(c)) {
    }

    void view() const override {
        std::string result = renderer->beginDocument();
        result += renderer->renderTitle(title);
        result += renderer->renderContent(content);
        result += renderer->endDocument();
        std::cout << result << "\n";
    }
};

class ProductPage : public Page {
    Product product;
public:
    ProductPage(Renderer* r, const Product& p) : Page(r), product(p) {}

    void view() const override {
        std::string result = renderer->beginDocument();
        result += renderer->renderProduct(product);
        result += renderer->endDocument();
        std::cout << result << "\n";
    }
};

int main() {
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "ukr");
    HTMLRenderer html;
    JsonRenderer json;
    XmlRenderer  xml;

    SimplePage spHtml(&html, "Ласкаво просимо", "Це мій сайт!");
    SimplePage spJson(&json, "Ласкаво просимо", "Це мій сайт!");

    Product laptop{ 1001, "Ноутбук Pro", "16 ГБ RAM, SSD 1 ТБ", "img/laptop.jpg" };
    ProductPage prodHtml(&html, laptop);
    ProductPage prodJson(&json, laptop);
    ProductPage prodXml(&xml, laptop);

    std::cout << "=== SimplePage (HTML) ===\n";
    spHtml.view();

    std::cout << "\n=== SimplePage (JSON) ===\n";
    spJson.view();

    std::cout << "\n=== ProductPage (HTML) ===\n";
    prodHtml.view();

    std::cout << "\n=== ProductPage (JSON) ===\n";
    prodJson.view();

    std::cout << "\n=== ProductPage (XML) ===\n";
    prodXml.view();

    return 0;
}