#include "given.h"
#include "vbc.h"

node *parse_factor(char **s)
{
    node *n = NULL;

    if(accept(s, '('))
    {
        n = parse_sum(s);
        if(!n)
            return NULL;
        if(!expect(s, ')'))
        {
            destroy_tree(n);
            return NULL;
        }
        return n;
    }
    if (isdigit(**s))
    {
        node tmp = {VAL, **s - '0', NULL, NULL};
        n = new_node(tmp);
        (*s)++;
        return n;
    }
    unexpected(**s);
    return NULL;
}

node *parse_term(char **s)
{
    node *lhs = parse_factor(s);
    if(!lhs)
        return NULL;
    while(accept(s, '*'))
    {
        node *rhs = parse_factor(s);
        if(!rhs)
        {
            destroy_tree(lhs);
            return NULL;
        }
        node tmp = {MULTI, 0, lhs, rhs};
        lhs = new_node(tmp);
    }
    return (lhs);
}

node *parse_sum(char **s)
{
    node *lhs = parse_term(s);
    if(!lhs)
        return NULL;
    while(accept(s, '+'))
    {
        node *rhs = parse_term(s);
        if(!rhs)
        {
            destroy_tree(lhs);
            return NULL;
        }
        node tmp = {ADD, 0, lhs, rhs};
        lhs = new_node(tmp);
    }
    return (lhs);
}

node *parse_expr(char *s)
{
    node *ret = parse_sum(&s);
    if (ret && *s)
    {
        unexpected(*s);
        destroy_tree(ret);
        return (NULL);
    }
    return ret;
}
