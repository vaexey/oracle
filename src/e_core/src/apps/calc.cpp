#include "oracle.h"
#include "menu.hpp"
#include "keys.hpp"
#include "calc.hpp"

namespace apps
{
    void calc::setup()
    {
        expr = "";
    }

    void calc::loop()
    {
        String topExpr = expr;
        String botExpr = "";

        if(topExpr.length() > 16)
        {
            topExpr = expr.substring(0, 16);
            botExpr = expr.substring(16, 32);
        }

        nc::lcd::blink(false);
        
        nc::lcd::setCursor(0,0);
        nc::lcd::print(topExpr);
        for(int i = topExpr.length(); i < 16; i++)
            nc::lcd::print(" ");

        nc::lcd::setCursor(0,1);
        nc::lcd::print(botExpr);
        for(int i = botExpr.length(); i < 16; i++)
            nc::lcd::print(" ");


        int nextx = expr.length();
        int nexty = 0;

        if(nextx > 15)
        {
            nexty++;
            nextx -= 16;
        }

        nc::lcd::setCursor(nextx, nexty);
        nc::lcd::blink(true);
    }

    void calc::setdown()
    {
        nc::lcd::blink(false);
    }

    void calc::on_char(char chr)
    {
        nc::beep(1);
        
        expr = expr + chr;
    }

    void calc::on_number(int number)
    {
        on_char('0' + number);
    }

    int eval(String expr)
    {
        int skip = 0;
        int operand = 0;
        char operation = ' ';

        for(auto x : expr)
        {
            if(x >= '0' && x <= '9')
            {
                operand *= 10;
                operand += (int)(x - '0');

                skip++;

                continue;
            }

            skip++;
            operation = x;

            break;
        }

        String remainder = expr.substring(skip);

        if(operation == '+')
        {
            return operand + eval(remainder);
        }

        if(operation == '-')
        {
            return operand - eval(remainder);
        }

        if(operation == '*')
        {
            return operand * eval(remainder);
        }

        if(operation == '/')
        {
            return operand / eval(remainder);
        }

        return operand;
    }

    void calc::on_calc()
    {
        nc::beep(1);

        auto result = eval(expr);

        expr = String(result);
    }

    void calc::on_key_down(int key)
    {
        switch(key)
        {
            case KEY_0:
                on_number(0);
                return;
            case KEY_1:
                on_number(1);
                return;
            case KEY_2:
                on_number(2);
                return;
            case KEY_3:
                on_number(3);
                return;
            case KEY_4:
                on_number(4);
                return;
            case KEY_5:
                on_number(5);
                return;
            case KEY_6:
                on_number(6);
                return;
            case KEY_7:
                on_number(7);
                return;
            case KEY_8:
                on_number(8);
                return;
            case KEY_9:
                on_number(9);
                return;
            case KEY_UP:
                on_char('+');
                return;
            case KEY_DOWN:
                on_char('-');
                return;
            case KEY_LEFT:
                on_char('*');
                return;
            case KEY_RIGHT:
                on_char('/');
                return;
            case KEY_ENTER:
                on_calc();
                return;
        }

        nc::beep(3);
    }
} // namespace apps
