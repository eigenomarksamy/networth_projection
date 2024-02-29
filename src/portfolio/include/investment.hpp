#ifndef INVESTMENT_HPP_
#define INVESTMENT_HPP_

#include <math.h>
#include <string>
#include "utils.hpp"

class Investment {
private:
    std::string m_name;
    std::string m_ticker;
    double_t m_purchase_price;
    double_t m_current_price;
    uint32_t m_quantity;

public:
    Investment() = default;
    Investment(const std::string& name, const std::string& ticker,
               double_t purchasePrice, double_t currentPrice, uint32_t quantity)
               : m_name(name), m_ticker(ticker),
                 m_purchase_price(purchasePrice),
                 m_current_price(currentPrice),
                 m_quantity(quantity) { }
    bool operator==(const Investment& other) const {
        return ((m_name == other.m_name) &&
                (m_ticker == other.m_ticker) &&
                (m_purchase_price == other.m_purchase_price) &&
                (m_current_price == other.m_current_price) &&
                (m_quantity == other.m_quantity));
    }
    bool operator!=(const Investment& other) const {
        return !(*this == other);
    }

    std::string getName() const { return this->m_name; }
    void setName(const std::string& name) { m_name = name; }
    std::string getTicker() const { return this->m_ticker; }
    void setTicker(const std::string& ticker) { m_ticker = ticker; }
    double_t getPurchasePrice() const { return this->m_purchase_price; }
    void setPurchasePrice(const double_t purchasePrice) { m_purchase_price = purchasePrice; }
    double_t getCurrentPrice() const { return this->m_current_price; }
    void setCurrentPrice(const double_t currentPrice) { m_current_price = currentPrice; }
    uint32_t getQuantity() const { return this->m_quantity; }
    void setQuantity(const uint32_t quantity) { m_quantity = quantity; }
};


struct Transaction {
    Date m_date;
    double_t m_fees;
    bool operator==(const Transaction& other) const {
        return ((m_date == other.m_date) && (m_fees == other.m_fees));
    }
};

class ComplexInvestment {

private:
    Investment m_investment;
    Transaction m_transaction;
    std::string m_id;

public:
    ComplexInvestment() = default;
    bool operator==(const ComplexInvestment& other) const {
        return ((m_investment == other.m_investment) &&
                (m_transaction == other.m_transaction) &&
                (m_id == other.m_id));
    }
    Investment getInvestment() const { return m_investment; }
    Transaction getTransaction() const { return m_transaction; }
    std::string getId() const { return m_id; }
    void setInvestment(const Investment& investment) { m_investment = investment; }
    void setTransaction(const Transaction& transaction) { m_transaction = transaction; }
    void setId(const std::string& id) { m_id = id; }
};

#endif /* INVESTMENT_HPP_ */