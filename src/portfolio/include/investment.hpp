#ifndef INVESTMENT_HPP_
#define INVESTMENT_HPP_

#include <math.h>
#include <string>
#include "utils.hpp"
#include "datetime.hpp"

class Investment {
private:
    std::string m_name;
    std::string m_ticker;
    double_t m_purchase_price;
    double_t m_current_price;
    double_t m_quantity;

public:
    Investment() = default;
    Investment(const std::string& name, const std::string& ticker,
               double_t purchasePrice, double_t currentPrice, double_t quantity)
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
    double_t getQuantity() const { return this->m_quantity; }
    void setQuantity(const double_t quantity) { m_quantity = quantity; }
};


struct Transaction {
    enum class Currency {USD, EUR};
    DateTime m_datetime;
    double_t m_fees;
    double_t m_conversion_fees;
    Currency m_currency;
    bool operator==(const Transaction& other) const {
        return ((m_datetime == other.m_datetime) &&
                (m_fees == other.m_fees) &&
                (m_conversion_fees == other.m_conversion_fees) &&
                (m_currency == other.m_currency));
    }
    bool operator!=(const Transaction& other) const {
        return !(*this == other);
    }
    std::string currency_to_str() const {
        return (m_currency == Currency::USD) ? "USD" : "EUR";
    }
    void str_to_currency(std::string& str) {
        convertStrToLowerCase(str);
        m_currency = (str == "usd") ? Currency::USD : Currency::EUR;
    }
};

class ComplexInvestment {

private:
    Investment m_investment;
    Transaction m_transaction;
    std::string m_id;
    uint32_t m_sequencer;
    double_t m_currency_conversion_rate;

public:
    ComplexInvestment() = default;
    ComplexInvestment(const ComplexInvestment& other) = default;
    ~ComplexInvestment() = default;
    bool operator==(const ComplexInvestment& other) const {
        return ((m_investment == other.m_investment) &&
                (m_transaction == other.m_transaction) &&
                (m_id == other.m_id) &&
                (m_sequencer == other.m_sequencer) &&
                (m_currency_conversion_rate == other.m_currency_conversion_rate));
    }
    bool operator!=(const ComplexInvestment& other) const {
        return !(*this == other);
    }
    Investment getInvestment() const { return m_investment; }
    Transaction getTransaction() const { return m_transaction; }
    std::string getId() const { return m_id; }
    uint32_t getSequencer() const { return m_sequencer; }
    double_t getCurrencyConversionRate() const { return m_currency_conversion_rate; }
    void setInvestment(const Investment& investment) { m_investment = investment; }
    void setTransaction(const Transaction& transaction) { m_transaction = transaction; }
    void setId(const std::string& id) { m_id = id; }
    void setSequencer(const uint32_t sequencer) { m_sequencer = sequencer; }
    void setCurrencyConversionRate(const double_t currency_conversion_rate) { m_currency_conversion_rate = currency_conversion_rate; }
};

#endif /* INVESTMENT_HPP_ */