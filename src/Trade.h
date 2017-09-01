#pragma once

#include "ITrade.h"
#include "Streamer.h"

struct Trade : ITrade
{
    virtual double quantity() const
    {
        return m_quantity;
    }

    void init(double quantity = 1.0)
    {
        m_quantity = quantity;
    }

private:
    virtual void print_details(std::ostream& os) const = 0;
    virtual void save_details(my_ofstream& os) const = 0;
    virtual void load_details(my_ifstream& is) = 0;

protected:
    virtual void print(std::ostream& os) const
    {
        os << "Id:         " << id()                << std::endl;
        os << "Name:       " << idname()            << std::endl;
        os << "Quantity:   " << quantity()          << std::endl;
        print_details(os);
        os << std::endl;
    }

    virtual void save(my_ofstream& os) const
    {
        os << id()
           << quantity();
        save_details(os);
    }

    virtual void load(my_ifstream& is)
    {
        // read everything but id
        is >> m_quantity;
        load_details(is);
    }

private:
    double m_quantity;
};
