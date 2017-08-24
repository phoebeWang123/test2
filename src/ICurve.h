#pragma once

#include <memory>

#include "IObject.h"
#include "Date.h"

// forward declarations
struct Market;
struct MarketDataServer;

struct ICurve : IObject
{
    virtual string name() const = 0;
    virtual Date today() const = 0;
};

// forward declaration
struct ICurveDiscount;

typedef std::shared_ptr<const ICurve> ptr_curve_t;
typedef std::shared_ptr<const ICurveDiscount> ptr_disc_curve_t;

struct ICurveDiscount : ICurve
{
    // compute the discount factor
    virtual double df(const Date& t) const = 0;
};

