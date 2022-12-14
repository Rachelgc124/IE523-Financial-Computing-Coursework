//
//  portfolio optimization.h
//  Programming Assignment6: Portfolio Optimization Software in C++ under Parallel Shifts in Term Structure
//  Created by Chang Gao on 2022/10/20.
//

#ifndef portfolio_optimization_h
#define portfolio_optimization_h

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <numeric>
#include "gurobi_c++.h"

using namespace std;

class portfolio_optimization
{
    // Private
    const double ERROR = 1e-10;
    
    char file_name[10];
    int number_of_cf;
    double debt_obligation_amount;
    double pv_of_debt_obligation;
    int debt_due;
    vector<double> price_list;
    vector<int> maturity_list;
    vector<vector<double>> cashflows;
    vector<double> duration;
    vector<double> convexity;
    vector<double> ytm;
    
    // private member function: check if r is reasonable
    // calculate the difference between the FV of the
    // price of the bond and the FV of  cash flows
    double my_function(vector<double> cashflow, double price, double rate)
    {
        double diff = price * pow((1 + rate), cashflow.size());
        for (int i = 0; i < cashflow.size(); i++)
            diff = diff - (cashflow.at(i) * pow((1 + rate), cashflow.size() - 1 - i));
        return diff;
    }
    
    // private member function: derivative of my_function
    double derivative_function(vector<double> cashflow, double price, double rate)
    {
        double result = cashflow.size() * price * pow((1 + rate), cashflow.size() - 1);
        for (int i = 0; i < cashflow.size() - 1; i++)
            result = result - ((cashflow.size() - 1 - i) * cashflow.at(i)
                               * pow((1 + rate), cashflow.size() - 2 - i));
        return result;
    }
    
    // private member function: Newton Raphson
    // find the root of my_function
    double Newton_Raphson(vector<double> cashflow, double price, double rate)
    {
        while (fabs(my_function(cashflow, price, rate)) > ERROR)
            rate = rate - my_function(cashflow, price, rate) / derivative_function(cashflow, price, rate);
        return rate;
    }
    
    // private member function: calculate duration
    double get_duration(vector<double> cashflow, double price, double rate)
    {
        double duration = .0;
        for (int i = 0; i < cashflow.size(); i++)
            duration = duration + cashflow.at(i) * (i + 1) / pow((1 + rate), i + 1);
        duration = duration / price;
        return duration;
    }
    
    double get_convexity(vector<double> cashflow, double price, double rate)
    {
        double convexity = .0;
        for (int i = 0; i < cashflow.size(); i++)
            convexity = convexity + (cashflow.at(i) * ((i + 1) * (i + 2)) / pow((1 + rate), i + 3));
        convexity = convexity / price;
        return convexity;
    }

    // private member function: reads data
    void get_data(int argc, const char * argv[])
    {
        // The first entry in the input file is the #CFs,
        // followed by k many bonds: the first lines of each row
        // is the PV of the bond, followed by the bond's cash flows,
        // followed by the FV of users' debt obligation and the
        // time when it is due
        string line;
        vector <string> lines;
        
        sscanf(argv[1], "%s", file_name);
        ifstream input_file(file_name);
        if (input_file.is_open())
        {
            while (getline(input_file, line, '\n'))
                lines.push_back(line);
        }
        else
            cout << "Input file does not exist in PWD." << endl;
        
        // read data from file
        number_of_cf = stoi(lines.at(0));
        for (int i = 0; i < number_of_cf; i++)
        {
            double temp;
            vector <double> temp1;
            stringstream ss(lines.at(i + 1));
            while (ss >> temp)
                temp1.push_back(temp);
            cashflows.push_back(temp1);
        }
        
        for (int i = 0; i < number_of_cf; i++)
        {
            price_list.push_back(cashflows[i][0]);
            cashflows.at(i).erase(cashflows.at(i).begin());
            maturity_list.push_back(cashflows[i][0]);
            cashflows.at(i).erase(cashflows.at(i).begin());
        }
        
        double temp;
        vector <double> temp1;
        stringstream ss(lines.at(number_of_cf + 1));
        while (ss >> temp)
            temp1.push_back(temp);
        debt_obligation_amount = temp1.at(0);
        debt_due = temp1.at(1);
        
        // characteristic
        for (int i = 0; i < number_of_cf; i++)
        {
            double r = Newton_Raphson(cashflows.at(i), price_list.at(i), 0.0);
            ytm.push_back(r);
            double d = get_duration(cashflows.at(i), price_list.at(i), r);
            duration.push_back(d);
            double c = get_convexity(cashflows.at(i), price_list.at(i), r);
            convexity.push_back(c);
        }
        
        double discount_rate = accumulate(ytm.begin(), ytm.end(), 0.0) / ytm.size();
        pv_of_debt_obligation  = debt_obligation_amount / pow((1 + discount_rate), debt_due);
    }
    
    // private member function: print data
    void print_data()
    {
        cout << "Input File: " << file_name << endl;
        cout << "We owe " << debt_obligation_amount << " in " << debt_due << " years" << endl;
        cout << "Number of Cash Flows: " << number_of_cf << endl;
        for (int i = 0; i < number_of_cf; i++)
        {
            cout << "------------------------------------------------------------------" << endl;
            cout << "Cash Flow #" << i + 1 << endl;
            cout << "Price = " << price_list.at(i) << endl;
            cout << "Maturity = " << maturity_list.at(i) << endl;
            cout << "Yield to Maturity = " << ytm.at(i) << endl;
            cout << "Duration = " << duration.at(i) << endl;
            cout << "Convexity = " << convexity.at(i) << endl;
        }
        cout << "******************************************************************" << endl;
    }
    
    // private member function: get optimal portfolio
    void get_optimal_portfolio()
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "Optimal_Portfolio.log");
        env.start();
        
        // Create an empty model
        GRBModel model = GRBModel(env);
        model.set(GRB_IntParam_LogToConsole, 0);
        
        // Create variables
        vector<GRBVar> lambdas;
        for (int i = 0; i < number_of_cf; i++)
        {
            GRBVar lambda = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS);
            lambdas.push_back(lambda);
        }
        
        // Set objective: maximize Convexity
        GRBLinExpr obj;
        for (int i = 0; i < number_of_cf; i++)
            obj.addTerms(&convexity.at(i), &lambdas.at(i), 1);
        model.setObjective(obj, GRB_MAXIMIZE);
        
        // Add constraint: duration of the portfolio = due date
        GRBLinExpr cst0;
        for (int i = 0; i < number_of_cf; i++)
            cst0.addTerms(&duration.at(i), &lambdas.at(i), 1);
        model.addConstr(cst0 == debt_due, "c0");
        
        // Add constraint: sum of lambdas = 1
        GRBLinExpr cst1;
        for (int i = 0; i < number_of_cf; i++)
            cst1 += lambdas.at(i);
        model.addConstr(cst1 == 1, "c1");
        
        // Optimize model
        model.optimize();
        
        if (GRB_OPTIMAL == model.get(GRB_IntAttr_Status))
        {
            GRBVar *vars = model.getVars();
            vector<double> opt_lambdas;
            for (int i = 0; i < number_of_cf; i++)
                opt_lambdas.push_back(vars[i].get(GRB_DoubleAttr_X));
            
            cout << "******************************************************************" << endl;
            cout << "Assuming no changes in term structure, the %-age of the face value " << endl;
            cout << "of each cash flow we would have to purchase to meet the future debt obligation is: " << endl;
            for (int i = 0; i < number_of_cf; i++)
            {
                cout << "%Face value of Cash-Flow " << i + 1 << ": ";
                cout << opt_lambdas.at(i) * pv_of_debt_obligation / price_list.at(i) << endl;
            }
            
            cout << "******************************************************************" << endl;
            cout << "The largest convexity we can get is : " << model.get(GRB_DoubleAttr_ObjVal) << endl;
            cout << "The optimal portfolio: " << endl;
            for (int i = 0; i < number_of_cf; i++)
                cout << "%Cash-Flow " << i + 1 << ": " << opt_lambdas.at(i) << endl;
            cout << endl;
            
            cout << "To immunize against small changes in 'r' for each $1 of PV, you should buy" << endl;
            for (int i = 0; i < number_of_cf; i++)
                cout << "$" << opt_lambdas.at(i) << " of Cash Flow# " << i + 1 << endl;
            cout << "If you need to immunize for a larger PV-value, just buy an appropriate proportion" << endl;
            cout << "******************************************************************" << endl;
            
            cout << "For example, if you want to immunize for $500 of PV, buy" << endl;
            for (int i = 0; i < number_of_cf; i++)
            {
                cout << "$" << 500 << " x " << opt_lambdas.at(i) << " = " << "$" << 500 * opt_lambdas.at(i);
                cout << " of Cash-Flow " << i + 1 << endl;
            }
            cout << "******************************************************************" << endl;
        }
        else
        {
            cout << "There is no portfolio that meets the duration constraint of ";
            cout << debt_due << " years" << endl;
            cout << "******************************************************************" << endl;
        }
    }
    
public:
    void optimized(int argc, const char * argv[])
    {
        get_data(argc, argv);
        
        print_data();
        
        get_optimal_portfolio();
    }
};

#endif /* portfolio_optimization_h */
