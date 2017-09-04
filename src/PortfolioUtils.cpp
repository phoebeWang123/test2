#include "Global.h"
#include "PortfolioUtils.h"
#include "TradePayment.h"

#include <numeric>

void print_portfolio(const portfolio_t& portfolio)
{
    std::for_each(portfolio.begin(), portfolio.end(), [](const ptrade_t& pt){ pt->print(std::cout); });
}

std::vector<ppricer_t> get_pricers(const std::vector<ptrade_t>& portfolio)
{
    std::vector<ppricer_t> pricers(portfolio.size());
    std::transform( portfolio.begin(), portfolio.end(), pricers.begin()
                  , [](const ptrade_t &pt) -> ppricer_t { return pt->pricer(); } );
    return pricers;
}

portfolio_values_t compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt)
{
    portfolio_values_t prices(pricers.size());
    std::transform(pricers.begin(), pricers.end(), prices.begin()
        , [&mkt](const ppricer_t &pp) -> double { return pp->price(mkt); });
    return prices;
}

double portfolio_total(const portfolio_values_t& values)
{
    return std::accumulate(values.begin(), values.end(), 0.0);
}

std::vector<std::pair<string, portfolio_values_t>> compute_pv01(const std::vector<ppricer_t>& pricers, Market& mkt)
{
    std::vector<std::pair<string, portfolio_values_t>> pv01;  // PV01 per trade

    const double bump_size = 0.01 / 100;
    const double scaler = 0.01 / 100;

    // filter risk factors related to IR
    auto base = mkt.get_risk_factors(ir_rate_prefix);

    // compute prices for perturbated markets and aggregate results
    pv01.reserve(base.size());
    for (auto d = base.begin(); d != base.end(); ++d) {
        std::vector<double> pv_up, pv_dn;
        std::vector<std::pair<string, double>> bumped(1, *d);
        pv01.push_back(std::make_pair(d->first, std::vector<double>(pricers.size())));

        // bump down and price
        bumped[0].second = d->second - bump_size;
        mkt.set_data_points(bumped);
        pv_dn = compute_prices(pricers, mkt);

        // bump up and price
        bumped[0].second = d->second + bump_size; // bump up
        mkt.set_data_points(bumped);
        pv_up = compute_prices(pricers, mkt);

        // reset market to its original state
        bumped[0].second = d->second;
        mkt.set_data_points(bumped);

        // compute estimator of the derivative via central finite differences
        std::transform(pv_up.begin(), pv_up.end(), pv_dn.begin(), pv01.back().second.begin()
            , [bump_size, scaler] (double hi, double lo)
                -> double { return (hi - lo) / (2.0 * bump_size) * scaler; });
    }

    return pv01;
}


ptrade_t load_trade(my_ifstream& is)
{
    string name;
    ptrade_t p;

    // read trade identifier
    guid_t id;
    is >> id;

    if (id == TradePayment::m_id)
        p.reset(new TradePayment);
    else
        THROW( "Unknown trade type:" << id );

    p->load(is);

    return p;
}

void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio)
{
    // test saving to file
    my_ofstream of(filename);
    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt)
        {
            pt->save(of);
            of.endl();
        });
    of.close();
}

std::vector<ptrade_t> load_portfolio(const string& filename)
{
    std::vector<ptrade_t> portfolio;

    // test reloading the portfolio
    my_ifstream is(filename);
    while(is.read_line())
        portfolio.push_back(load_trade(is));

    return portfolio;
}
