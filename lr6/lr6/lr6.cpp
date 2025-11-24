#include <iostream>
#include <string>
#include <unordered_map>

class Downloader {
public:
    virtual ~Downloader() = default;
    virtual std::string download(const std::string& url) = 0;
};

class SimpleDownloader : public Downloader {
public:
    std::string download(const std::string& url) override {
        std::cout << "    [SimpleDownloader] Завантажую з мережі: " << url << std::endl;
        return "Дані зображення з " + url + " (розмір ~150KB)";
    }
};

class CachingDownloaderProxy : public Downloader {
private:
    SimpleDownloader realDownloader;
    std::unordered_map<std::string, std::string> cache;

public:
    std::string download(const std::string& url) override {
        auto it = cache.find(url);
        if (it != cache.end()) {
            std::cout << "    [CACHE HIT] Повертаю з кешу: " << url << std::endl;
            return it->second;
        }

        std::cout << "    [CACHE MISS] Завантажую через реальний об'єкт: " << url << std::endl;
        std::string content = realDownloader.download(url);
        cache[url] = content;
        return content;
    }
};

void renderPage(Downloader& downloader, const std::string& pageTitle, const std::string& imageUrl) {
    std::cout << "\n=== Рендер сторінки: \"" << pageTitle << "\" ===\n";
    std::string imageData = downloader.download(imageUrl);
    std::cout << "    Зображення отримано! (розмір: " << imageData.size() << " символів)\n";
}

int main() {
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "ukr");
    std::cout << "1. Використовуємо простий завантажувач (БЕЗ кешу)\n";
    SimpleDownloader simple;
    renderPage(simple, "Головна сторінка", "https://site.com/logo.png");
    renderPage(simple, "Про нас", "https://site.com/logo.png");
    renderPage(simple, "Контакти", "https://site.com/map.jpg");

    std::cout << "\n----------------------------------------------------\n\n";

    std::cout << "2. Використовуємо Замісник з кешуванням\n";
    CachingDownloaderProxy proxy;
    renderPage(proxy, "Головна", "https://site.com/logo.png");
    renderPage(proxy, "Про компанію", "https://site.com/logo.png");
    renderPage(proxy, "Галерея", "https://site.com/photo1.jpg");
    renderPage(proxy, "Головна ще раз", "https://site.com/logo.png");

    std::cout << "\nРобота завершена.\n";
    return 0;
}