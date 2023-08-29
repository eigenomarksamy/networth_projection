#ifndef INVESTMENT_HPP_
#define INVESTMENT_HPP_

#include <math.h>
#include <string>

class Investment {
private:
    std::string m_name;
    std::string m_ticker;
    double_t m_purchase_price;
    uint32_t m_quantity;

public:
    Investment(const std::string& name, const std::string& ticker,
               double_t purchasePrice, uint32_t quantity)
               : m_name(name), m_ticker(ticker),
                 m_purchase_price(purchasePrice), m_quantity(quantity) { }

    std::string getName() const { return this->m_name; }
    void setName(const std::string& name) { m_name = name; }
    std::string getTicker() const { return this->m_ticker; }
    void setTicker(const std::string& ticker) { m_ticker = ticker; }
    double_t getPurchasePrice() const { return this->m_purchase_price; }
    void setPurchasePrice(const double_t purchasePrice) { m_purchase_price = purchasePrice; }
    uint32_t getQuantity() const { return this->m_quantity; }
    void setQuantity(const uint32_t quantity) { m_quantity = quantity; }
};


#endif /* INVESTMENT_HPP_ */