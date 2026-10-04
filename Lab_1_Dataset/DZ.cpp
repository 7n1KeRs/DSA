#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <ctime>
#include <iomanip>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <cmath>

const int NUM_SHOPS = 30;

struct Shop {
    std::string name;
    std::string type;
    double latitude;
    double longitude;
    int openHour;
    int closeHour;
    std::vector<std::string> allowedCategories;
};

struct BrandInfo {
    std::string name;
    double priceMultiplier;   
};

struct CategoryInfo {
    std::string name;
    std::string shopType;
    double minPrice;          
    double maxPrice;          
    std::vector<BrandInfo> brands;
};

std::mt19937 rng(std::time(nullptr));

int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

double randomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

double roundTo8(double value) {
    return std::round(value * 1e8) / 1e8;
}

struct ShopTemplate {
    std::string name;
    std::string type;
};

std::vector<ShopTemplate> generateShopTemplates() {
    return {
        {"Магнит", "Продукты"}, {"Пятерочка", "Продукты"}, {"Перекресток", "Продукты"},
        {"Лента", "Продукты"}, {"Окей", "Продукты"}, {"ВкусВилл", "Продукты"},
        {"Ашан", "Продукты"}, {"Дикси", "Продукты"}, {"Монетка", "Продукты"},
        {"Верный", "Продукты"}, {"Красное & Белое", "Продукты"}, {"Бристоль", "Продукты"},
        {"РеалЪ", "Продукты"}, {"Семишагофф", "Продукты"}, {"Спар", "Продукты"},
        {"Азбука Вкуса", "Продукты"}, {"Глобус", "Продукты"}, {"Метро", "Продукты"},
        {"Ценбери", "Продукты"},
        {"ДНС", "Электроника"}, {"М.Видео", "Электроника"}, {"Эльдорадо", "Электроника"},
        {"Ситилинк", "Электроника"}, {"Регард", "Электроника"}, {"Технопарк", "Электроника"},
        {"Леруа Мерлен", "Стройматериалы"}, {"OBI", "Стройматериалы"}, {"Петрович", "Стройматериалы"},
        {"Спортмастер", "Спорт"}, {"Аптека 36.6", "Аптека"}
    };
}

struct CategoryTemplate {
    std::string name;
    std::string shopType;
    double minPrice;
    double maxPrice;
    std::vector<std::pair<std::string, double>> brands;
};

std::vector<CategoryTemplate> generateCategoryTemplates() {
    return {
        {"Молоко", "Продукты", 50, 150, {{"Простоквашино", 1.2}, {"Домик в деревне", 1.1}, {"Вкуснотеево", 1.0}, {"Молочная ферма", 0.9}}},
        {"Хлеб", "Продукты", 30, 100, {{"Бородинский", 1.0}, {"Каравай", 1.1}, {"Хлебный дом", 1.2}, {"Рижский", 0.9}}},
        {"Сыр", "Продукты", 200, 800, {{"Российский", 1.0}, {"Гауда", 1.3}, {"Пармезан", 2.0}, {"Чеддер", 1.5}}},
        {"Мясо", "Продукты", 300, 1200, {{"Мираторг", 1.3}, {"Останкино", 1.1}, {"Черкизово", 1.0}, {"Дымов", 1.2}}},
        {"Рыба", "Продукты", 200, 1500, {{"Русское море", 1.2}, {"Балтийский берег", 1.1}, {"Санта Бремор", 1.3}}},
        {"Овощи", "Продукты", 30, 200, {{"Белая дача", 1.2}, {"Эко-ферма", 1.3}, {"Тепличный", 1.0}}},
        {"Фрукты", "Продукты", 50, 300, {{"Азбука фруктов", 1.1}, {"Фруктовая лавка", 1.0}, {"Эдем", 1.2}}},
        {"Чай", "Продукты", 80, 600, {{"Greenfield", 1.3}, {"Lipton", 1.0}, {"Ahmad", 1.1}, {"Tess", 1.2}}},
        {"Кофе", "Продукты", 150, 1500, {{"Jacobs", 1.0}, {"Nescafe", 0.9}, {"Lavazza", 1.5}, {"Jardin", 1.1}}},
        {"Соки", "Продукты", 60, 300, {{"Добрый", 1.0}, {"Rich", 1.3}, {"Моя семья", 1.1}, {"J7", 1.2}}},
        {"Вода", "Продукты", 20, 200, {{"Святой источник", 1.0}, {"Боржоми", 2.0}, {"Aqua Minerale", 1.1}}},
        {"Конфеты", "Продукты", 100, 800, {{"Коркунов", 1.5}, {"Бабаевский", 1.3}, {"Рот Фронт", 1.0}, {"Красный Октябрь", 1.2}}},
        {"Печенье", "Продукты", 50, 400, {{"Юбилейное", 1.0}, {"Oreo", 1.3}, {"Яшкино", 0.9}, {"Slakon", 1.1}}},
        {"Мороженое", "Продукты", 50, 500, {{"Золотой стандарт", 1.0}, {"Чистая линия", 1.2}, {"Carte D'Or", 1.5}}},
        {"Соусы", "Продукты", 60, 400, {{"Heinz", 1.4}, {"Calve", 1.1}, {"Балтимор", 1.0}, {"Махеев", 1.2}}},

        {"Ноутбук", "Электроника", 30000, 150000, {{"Lenovo", 1.0}, {"HP", 1.1}, {"Dell", 1.2}, {"Asus", 0.9}, {"Acer", 0.8}, {"Apple", 2.0}, {"MSI", 1.3}}},
        {"Смартфон", "Электроника", 10000, 120000, {{"Apple", 2.0}, {"Samsung", 1.4}, {"Xiaomi", 0.8}, {"Huawei", 1.0}, {"Google", 1.3}, {"OnePlus", 1.1}}},
        {"Телевизор", "Электроника", 20000, 200000, {{"Samsung", 1.3}, {"LG", 1.2}, {"Sony", 1.5}, {"Philips", 1.0}, {"Panasonic", 1.1}, {"TCL", 0.8}}},
        {"Наушники", "Электроника", 1000, 50000, {{"Sony", 1.4}, {"Bose", 1.8}, {"JBL", 1.0}, {"Sennheiser", 1.5}, {"Apple", 1.7}, {"HyperX", 0.9}}},
        {"Планшет", "Электроника", 15000, 90000, {{"Apple", 2.0}, {"Samsung", 1.3}, {"Lenovo", 0.9}, {"Huawei", 1.0}, {"Xiaomi", 0.8}}},
        {"Монитор", "Электроника", 8000, 80000, {{"Dell", 1.3}, {"LG", 1.1}, {"Samsung", 1.2}, {"AOC", 0.8}, {"BenQ", 1.0}, {"Asus", 1.1}}},
        {"Клавиатура", "Электроника", 1000, 15000, {{"Logitech", 1.1}, {"Razer", 1.4}, {"Corsair", 1.3}, {"HyperX", 1.2}, {"SteelSeries", 1.5}}},
        {"Мышь", "Электроника", 500, 10000, {{"Logitech", 1.1}, {"Razer", 1.4}, {"Corsair", 1.3}, {"SteelSeries", 1.5}, {"HyperX", 1.2}}},
        {"Принтер", "Электроника", 5000, 50000, {{"HP", 1.0}, {"Canon", 1.1}, {"Epson", 1.2}, {"Brother", 0.9}, {"Xerox", 1.3}}},
        {"Сканер", "Электроника", 3000, 30000, {{"Canon", 1.1}, {"Epson", 1.2}, {"HP", 1.0}, {"Brother", 0.9}}},
        {"Проектор", "Электроника", 15000, 150000, {{"Epson", 1.2}, {"BenQ", 1.0}, {"Optoma", 1.1}, {"ViewSonic", 0.9}}},
        {"Фотоаппарат", "Электроника", 20000, 300000, {{"Canon", 1.2}, {"Nikon", 1.3}, {"Sony", 1.4}, {"Fujifilm", 1.1}, {"Olympus", 1.0}}},
        {"Игровая приставка", "Электроника", 20000, 70000, {{"Sony", 1.2}, {"Microsoft", 1.1}, {"Nintendo", 1.0}}},
        {"Роутер", "Электроника", 1000, 20000, {{"TP-Link", 0.8}, {"Asus", 1.3}, {"Keenetic", 1.2}, {"MikroTik", 1.5}, {"D-Link", 0.9}}},
        {"Внешний диск", "Электроника", 2000, 20000, {{"Seagate", 1.0}, {"WD", 1.1}, {"Toshiba", 0.9}, {"Samsung", 1.2}, {"Kingston", 1.0}}},

        {"Холодильник", "Бытовая техника", 20000, 200000, {{"Bosch", 1.5}, {"LG", 1.2}, {"Samsung", 1.3}, {"Indesit", 0.8}, {"Atlant", 0.7}, {"Haier", 1.0}}},
        {"Стиральная машина", "Бытовая техника", 15000, 120000, {{"Bosch", 1.5}, {"LG", 1.2}, {"Samsung", 1.3}, {"Indesit", 0.8}, {"Candy", 0.9}, {"Electrolux", 1.4}}},
        {"Пылесос", "Бытовая техника", 3000, 80000, {{"Dyson", 2.5}, {"Bosch", 1.4}, {"Philips", 1.0}, {"Samsung", 1.2}, {"Rowenta", 1.1}}},
        {"Микроволновка", "Бытовая техника", 4000, 30000, {{"Samsung", 1.2}, {"LG", 1.1}, {"Bosch", 1.4}, {"Panasonic", 1.3}, {"Sharp", 1.0}}},
        {"Кофемашина", "Бытовая техника", 15000, 150000, {{"DeLonghi", 1.2}, {"Jura", 2.0}, {"Nivona", 1.3}, {"Melitta", 1.0}, {"Philips", 0.9}}},
        {"Утюг", "Бытовая техника", 1000, 15000, {{"Philips", 1.0}, {"Bosch", 1.3}, {"Tefal", 1.1}, {"Rowenta", 1.2}, {"Braun", 1.4}}},
        {"Фен", "Бытовая техника", 800, 20000, {{"Philips", 1.0}, {"Bosch", 1.3}, {"Rowenta", 1.2}, {"Dyson", 3.0}, {"Braun", 1.4}}},
        {"Блендер", "Бытовая техника", 1500, 25000, {{"Bosch", 1.3}, {"Philips", 1.0}, {"Tefal", 1.1}, {"Redmond", 0.9}, {"Polaris", 0.8}}},
        {"Мультиварка", "Бытовая техника", 3000, 30000, {{"Redmond", 1.0}, {"Polaris", 0.9}, {"Philips", 1.2}, {"Panasonic", 1.4}}},
        {"Электрочайник", "Бытовая техника", 800, 10000, {{"Bosch", 1.3}, {"Philips", 1.0}, {"Tefal", 1.1}, {"Redmond", 0.9}, {"Polaris", 0.8}}},

        {"Краска", "Стройматериалы", 500, 5000, {{"Tikkurila", 1.4}, {"Dulux", 1.2}, {"Caparol", 1.3}, {"Текс", 0.8}}},
        {"Обои", "Стройматериалы", 300, 5000, {{"Rasch", 1.5}, {"Erismann", 1.3}, {"АС", 0.9}, {"Винил", 0.8}}},
        {"Плитка", "Стройматериалы", 500, 3000, {{"Kerama Marazzi", 1.2}, {"Cersanit", 1.0}, {"Lasselsberger", 1.1}}},
        {"Ламинат", "Стройматериалы", 700, 4000, {{"Tarkett", 1.2}, {"Kronospan", 1.0}, {"Quick-Step", 1.5}}},
        {"Инструмент", "Стройматериалы", 1000, 30000, {{"Bosch", 1.4}, {"Makita", 1.5}, {"DeWalt", 1.6}, {"Зубр", 0.7}}},

        {"Кроссовки", "Спорт", 3000, 25000, {{"Nike", 1.3}, {"Adidas", 1.2}, {"Puma", 1.0}, {"Reebok", 0.9}, {"New Balance", 1.1}}},
        {"Одежда", "Спорт", 1500, 15000, {{"Nike", 1.3}, {"Adidas", 1.2}, {"Puma", 1.0}, {"Reebok", 0.9}}},
        {"Тренажёр", "Спорт", 10000, 100000, {{"Technogym", 2.0}, {"Life Fitness", 1.8}, {"NordicTrack", 1.3}}},

        {"Лекарства", "Аптека", 100, 3000, {{"Фармстандарт", 1.1}, {"Отисифарм", 1.2}, {"Валента", 1.0}}},
        {"Витамины", "Аптека", 200, 3000, {{"Solgar", 1.5}, {"Now Foods", 1.4}, {"Компливит", 0.8}, {"Алфавит", 1.0}}}
    };
}

std::string generateCardNumber(std::map<std::string, int>& cardUsage) {
    for (int attempt = 0; attempt < 1000; ++attempt) {
        std::string card = "";
        for (int i = 0; i < 4; ++i) {
            card += std::to_string(randomInt(1000, 9999));
            if (i < 3) card += " ";
        }
        if (cardUsage[card] < 5) {
            cardUsage[card]++;
            return card;
        }
    }
    return "0000 0000 0000 0000";
}

int main() {
    std::cout << "Генерация" << std::endl;
	std::cout << "Введите размер таблицы: ";
	int table_size;
	std::cin >> table_size;
	
    std::vector<ShopTemplate> shopTemplates = generateShopTemplates();
    std::vector<CategoryTemplate> categoryTemplates = generateCategoryTemplates();

    std::vector<CategoryInfo> catalog;
    for (const auto& ct : categoryTemplates) {
        CategoryInfo cat;
        cat.name = ct.name;
        cat.shopType = ct.shopType;
        cat.minPrice = ct.minPrice;
        cat.maxPrice = ct.maxPrice;
        for (const auto& b : ct.brands) {
            BrandInfo bi;
            bi.name = b.first;
            bi.priceMultiplier = b.second;
            cat.brands.push_back(bi);
        }
        catalog.push_back(cat);
    }

    std::vector<Shop> shops;
    for (int i = 0; i < NUM_SHOPS && i < (int)shopTemplates.size(); ++i) {
        Shop s;
        s.name = shopTemplates[i].name;
        s.type = shopTemplates[i].type;
        s.latitude = roundTo8(randomDouble(59.800000, 60.000000));
        s.longitude = roundTo8(randomDouble(30.100000, 30.500000));
        s.openHour = randomInt(8, 10);
        s.closeHour = randomInt(20, 23);

        for (const auto& cat : catalog) {
            if (cat.shopType == s.type) {
                s.allowedCategories.push_back(cat.name);
            }
        }
        if (s.allowedCategories.size() > 10) {
            std::shuffle(s.allowedCategories.begin(), s.allowedCategories.end(), rng);
            s.allowedCategories.resize(10);
        }
        shops.push_back(s);
    }

    std::vector<std::string> allLines;
    std::map<std::string, int> cardUsage;
    std::map<std::string, std::set<int>> shopReceipts;

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    int receiptCounter = 0;

    while ((int)allLines.size() < table_size) {
        receiptCounter++;

        int shopIdx = randomInt(0, NUM_SHOPS - 1);
        Shop& currentShop = shops[shopIdx];

        int year = currentYear;
        int month = currentMonth;
        int day = currentDay;
        if (randomInt(0, 1) == 0 && currentDay > 1) {
            day = currentDay - 1;
        }

        int hour = randomInt(currentShop.openHour, currentShop.closeHour - 1);
        int minute = randomInt(0, 59);
        int second = randomInt(0, 59);

        char dateTimeBuffer[35];
        sprintf(dateTimeBuffer, "%04d-%02d-%02dT%02d:%02d:%02d+03:00", year, month, day, hour, minute, second);
        std::string dateTime = dateTimeBuffer;

        std::stringstream coordStream;
        coordStream << std::fixed << std::setprecision(8)
                    << currentShop.latitude << ", " << currentShop.longitude;
        std::string coordinates = coordStream.str();

        int receiptNum;
        do {
            receiptNum = randomInt(1000, 9999);
        } while (shopReceipts[currentShop.name].count(receiptNum) > 0);
        shopReceipts[currentShop.name].insert(receiptNum);

        std::string card = generateCardNumber(cardUsage);

        int itemsInReceipt = randomInt(2, 5);

        std::vector<std::string> receiptLines;
        int addedItems = 0;
        int attempts = 0;

        while (addedItems < 2 && attempts < 20) {
            attempts++;

            if (currentShop.allowedCategories.empty()) break;
            std::string catName = currentShop.allowedCategories[randomInt(0, currentShop.allowedCategories.size() - 1)];

            auto it = std::find_if(catalog.begin(), catalog.end(), [&](const CategoryInfo& c){ return c.name == catName; });
            if (it == catalog.end()) continue;

            BrandInfo brand = it->brands[randomInt(0, it->brands.size() - 1)];

            double basePrice = randomDouble(it->minPrice, it->maxPrice);
            double shopMultiplier = 1.0;
            if (currentShop.name.find("Ашан") != std::string::npos) shopMultiplier = 0.95;
            if (currentShop.name.find("ВкусВилл") != std::string::npos) shopMultiplier = 1.10;
            if (currentShop.name.find("Азбука Вкуса") != std::string::npos) shopMultiplier = 1.25;

            double price = basePrice * brand.priceMultiplier * shopMultiplier;
            int quantity = randomInt(2, 5);
            double itemTotal = price * quantity;

            std::stringstream ss;
            ss << currentShop.name << ";"
               << dateTime << ";"
               << coordinates << ";"
               << catName << ";"
               << brand.name << ";"
               << std::fixed << std::setprecision(2) << price << ";"
               << card << ";"
               << quantity << ";"
               << receiptNum << ";"
               << std::fixed << std::setprecision(2) << itemTotal;
            receiptLines.push_back(ss.str());
            addedItems++;

            if (addedItems >= itemsInReceipt) break;
        }

        if (addedItems < 2) continue;

        for (const auto& line : receiptLines) {
            allLines.push_back(line);
        }
    }

    std::shuffle(allLines.begin(), allLines.end(), rng);

    std::ofstream outFile("DZ_Data_Set.csv");
    outFile << "Название магазина;Дата и Время;Координаты;Категория;Бранд;Цена;Номер карточки;Количество;Номер чека;Общая сумма\n";
    for (const auto& line : allLines) {
        outFile << line << "\n";
    }
    outFile.close();

    std::cout << "Готово!" << std::endl;

    return 0;
}