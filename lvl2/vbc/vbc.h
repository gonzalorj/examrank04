#ifndef VBC_H
# define VBC_H

#include "given.h"

node *parse_factor(char **s);
node *parse_term(char **s);
node *parse_sum(char **s);
node *parse_expr(char *s);
node    *new_node(node n);
void    destroy_tree(node *n);
void    unexpected(char c);
int accept(char **s, char c);
int expect(char **s, char c);
int eval_tree(node *tree);

#endif