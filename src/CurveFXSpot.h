#include "ICurve.h"

namespace minirisk{

struct CurveFXSpot : ICurveFXSpot
{
public:
    CurveFXSpot(Market* mkt, const Date & today, string & curve_name);
    virtual Date today() const { return m_today; }
    virtual string name() const { return m_name; }
    double spot() const; // compute the spot price
    
private:
    Date  m_today;
    string m_name;
    Market * m_mkt;
};

} // namespace minirisk
