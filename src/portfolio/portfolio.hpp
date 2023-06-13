#ifndef PORTFOLIO_HPP_
#define PORTFOLIO_HPP_

class Portfolio {

public:
    Portfolio() {}
    ~Portfolio() {}

private:
    void updatePortfolio();
    bool isValidPortfolio() const;
};


#endif /* PORTFOLIO_HPP_ */