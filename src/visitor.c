#include "../include/visitor.h"
#include "../include/abstract_syntax_tree.h"
#include "../include/scope.h"
#include <stdio.h>
#include <string.h>

static void print_value(AST *visited, char *buffer, size_t buffer_size)
{
    // printf("PRINT VALUE PROB: %s", visited->string_value);
    switch (visited->type)
    {
    case AST_STRING:
        if (strstr(visited->string_value, "\\n") != NULL)
        {
            char *ptr = visited->string_value;
            char *next;
            while ((next = strstr(ptr, "\\n")) != NULL)
            {
                strncat(buffer, ptr, next - ptr);
                strncat(buffer, "\n", buffer_size - strlen(buffer) - 1);
                ptr = next + 2;
            }
            break;
        }
        strncat(buffer, visited->string_value, buffer_size - strlen(buffer) - 1);
        break;
    case AST_INT:
        char int_str[20];
        snprintf(int_str, sizeof(int_str), "%d", visited->int_value);
        strncat(buffer, int_str, buffer_size - strlen(buffer) - 1);
        break;
    case AST_FLOAT:
        char float_str[20];
        snprintf(float_str, sizeof(float_str), "%.2f", visited->float_value);
        strncat(buffer, float_str, buffer_size - strlen(buffer) - 1);
        break;
    case AST_ARRAY:
        strncat(buffer, "[", buffer_size - strlen(buffer) - 1);
        for (int j = 0; j < visited->array_elements_size; j++)
        {
            print_value(visited->array_elements[j], buffer, buffer_size);
            if (j < visited->array_elements_size - 1)
            {
                strncat(buffer, ", ", buffer_size - strlen(buffer) - 1);
            }
        }

        strncat(buffer, "]", buffer_size - strlen(buffer) - 1);
        break;
    case AST_OBJECT:
        strncat(buffer, "{", buffer_size - strlen(buffer) - 1);
        for (int k = 0; k < visited->object_size; k++)
        {
            strncat(buffer, visited->object_pairs[k]->key, buffer_size - strlen(buffer) - 1);
            strncat(buffer, ": ", buffer_size - strlen(buffer) - 1);

            print_value(visited->object_pairs[k]->value, buffer, buffer_size);

            if (k < visited->object_size - 1)
            {
                strncat(buffer, ", ", buffer_size - strlen(buffer) - 1);
            }
        }
        strncat(buffer, "}", buffer_size - strlen(buffer) - 1);
        break;
    default:
        char ptr_str[20];
        snprintf(ptr_str, sizeof(ptr_str), "%p", visited);
        strncat(buffer, ptr_str, buffer_size - strlen(buffer) - 1);
        break;
    }
}

static AST *builtin_ebullience(AST **args, int args_size)
{
    char buffer[1024] = {0};
    for (int i = 0; i < args_size; i++)
    {
        AST *visited = vist(args[i]);
        print_value(visited, buffer, sizeof(buffer));

        if (i < args_size - 1)
        {
            if (visited->type == AST_STRING && strstr(visited->string_value, "\\n") != NULL)
                continue;

            strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        }
    }

    printf("%s", buffer);

    return init_ast(AST_NOOP);
}

static AST *builtin_exit()
{
    exit(0);
}

AST *vist(AST *child)
{
    // printf("%s\n", child->var_def_value);
    // printf("\n%s", child->var_name);
    // printf("CHILD: %s", child->string_value);
    switch (child->type)
    {
    case AST_VAR_DEF:
        return vist_var_def(child);
    case AST_VAR:
        return vist_var(child);
    case AST_REDEF_VAR:
        return vist_var_reassign(child);
    case AST_ARRAY_ACCSESS:
        return vist_array_accses(child);
    case AST_FN_CALL:
        return vist_fn_call(child);
    case AST_STRING:
        return vist_string(child);
    case AST_INT:
        return vist_int(child);
    case AST_FLOAT:
        return vist_float(child);
    case AST_ARRAY:
        return vist_array(child);
    case AST_OBJECT:
        return vist_object(child);
    case AST_COMPOUND:
        return vist_compund(child);
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
        // printf("TYPE: %d", child->type);
        return vist_binop(child);
    case AST_AX:
        return vist_conditional(child);
    case AST_PETRICHOR:
        return vist_loop_petrichor(child);
    case AST_INCENDIARY:
        return vist_loop_incendiary(child);
    case AST_NOOP:
        return child;
    }

    printf("uncaught statement %d", child->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST *vist_var_def(AST *child)
{
    // printf("Visiting variable: %s\n", child->var_name);

    // for (int i = 0; i < visitor->var_defs_size; i++)
    // {
    //     printf("Variable %d: %s\n", i, visitor->var_defs[i]->var_def_name);
    // }
    add_var_def(child->scope, child);

    return child;
}

AST *vist_var(AST *child)
{
    AST *var_def = get_var_def(child->scope, child->var_name);
    // printf("%s", va_list)
    // printf("YHYHYH: %s", var_def->var_def_value);
    if (var_def == (void *)0)
    {
        printf("Undefined variable: '%s'\n", child->var_name);
        exit(1);
    }

    // printf("VAR_REDEF: %d\n", child->var_def_value->int_value);
    // if (child->type == AST_REDEF_VAR)
    // {
    //     AST *new_value = vist(child->var_def_value);
    //     var_def->var_def_value = new_value;
    //     return var_def;
    // }

    return vist(var_def->var_def_value);
}

AST *vist_var_reassign(AST *child)
{
    if (child->var_def_value->var_redef_var != NULL)
    {
        AST *var = get_var_def(child->scope, child->var_def_value->var_name);
        // AST *idk = vist_var_reassign(var->var_redef_var);
        // printf("LOLLLL: %d + %d", var->var_def_value->int_value, child->var_def_value->var_redef_var->right->int_value);
        if(child->var_def_value->var_redef_var->right != NULL){
            child->var_def_value->var_redef_var->left = var->var_def_value;
        }else{
            child->var_def_value->var_redef_var = var->var_def_value;
        }
        AST *new_value = vist(child->var_def_value->var_redef_var);
        AST *var_def = reassign_var_def(child->scope, child->var_name, new_value);
        return var_def;
        // new_value = vist_binop(child);
    }
    // printf("\n\nNJCSCSOIUJ: %d", child->var_def_value->int_value);
    AST *new_value = vist(child->var_def_value);

    AST *var_def = reassign_var_def(child->scope, child->var_name, new_value);

    return var_def;
}

AST *vist_fn_call(AST *child)
{
    // printf("hit fn call");
    if (strcmp(child->fn_name, "ebullience") == 0)
    {
        return builtin_ebullience(child->fn_call_args, child->fn_call_args_size);
    }
    if (strcmp(child->fn_name, "exit") == 0)
    {
        return builtin_exit();
    }

    printf("undefined method %s", child->fn_name);
    exit(1);
}

AST *vist_conditional(AST *child)
{
    AST *condition_result = vist_relation(child->condition);

    if (condition_result->type == AST_INT && condition_result->int_value)
    {
        vist(child->then_branch);
    }
    else if (child->else_if_branch != NULL)
    {
        vist_conditional(child->else_if_branch);
    }
    else if (child->else_branch != NULL)
    {
        vist(child->else_branch->then_branch);
    }

    return init_ast(AST_NOOP);
}

AST *vist_relation(AST *child)
{   
    AST *left = vist(child->left);
    AST *right = NULL;
    
    if(child->right != NULL)
        right = vist(child->right);

    AST *result = init_ast(AST_INT);
    result->int_value = 0; // Default to false (0)

    switch (child->type)
    {
    case AST_LT:
        result->int_value = (left->int_value < right->int_value);
        break;
    case AST_GT:
        result->int_value = (left->int_value > right->int_value);
        break;
    case AST_GTE:
        result->int_value = (left->int_value >= right->int_value);
        break;
    case AST_LTE:
        result->int_value = (left->int_value <= right->int_value);
        break;
    case AST_EQ_EQ:
        result->int_value = (left->int_value == right->int_value);
        break;
    case AST_NEQ:
        result->int_value = (left->int_value != right->int_value);
        break;
    case AST_TRUE:
        result->int_value = left->int_value;
        break;
    case AST_FALSE:
         result->int_value = left->int_value;
         break;
    default:
        printf("Unsupported condition type: %d\n", child->type);
        exit(1);
    }

    return result;
}

AST *vist_loop_petrichor(AST *child)
{
    AST *init = vist(child->loop_init);
    char *var_name = child->loop_init->var_def_name;
    AST *var_def = get_var_def(child->scope, var_name);

    while (vist_relation(child->condition)->int_value)
    {
        vist(child->loop_body);

        if (child->loop_break)
            break;

        if (var_def)
            var_def->var_def_value->int_value++;
    }

    return init_ast(AST_NOOP);
}

AST *vist_loop_incendiary(AST *child)
{
    AST *var_def = (void *)0;

    if (child->loop_init && child->loop_init->var_def_name)
    {
        char *var_name = child->loop_init->var_def_name;
        var_def = get_var_def(child->scope, var_name);
    }

    while (vist_relation(child->condition)->int_value)
    {
        vist(child->loop_body);

        if (child->loop_break)
            break;

        if (var_def)
            var_def->var_def_value->int_value++;
    }
    return init_ast(AST_NOOP);
}

AST *vist_string(AST *child)
{
    return child;
}

AST *vist_int(AST *child)
{
    return child;
}

AST *vist_float(AST *child)
{
    return child;
}

AST *vist_array(AST *child)
{
    return child;
}

AST *vist_array_accses(AST *child)
{
    printf("hit");
    AST *array = get_var_def(child->scope, child->var_name)->var_def_value;
    int index = vist(child->array_index)->int_value;
    if (index < 0 || index >= array->array_elements_size)
    {
        printf("Error: Array index out of bounds\n");
        exit(1);
    }
    return array->array_elements[index];
}

AST *vist_object(AST *child)
{
    // for (int i = 0; i < child->object_size; i++)
    // {
    //     // Assuming each object pair has a key and a value
    //     printf("Key: %s\n", child->object_pairs[i]->key);

    //     // Visit the value associated with the key
    //     AST *value = vist(child->object_pairs[i]->value);

    //     // Optionally, handle the value depending on its type
    //     switch (value->type)
    //     {
    //     case AST_STRING:
    //         printf("Value: %s\n", value->string_value);
    //         break;
    //     case AST_INT:
    //         printf("Value: %d\n", value->int_value);
    //         break;
    //     case AST_FLOAT:
    //         printf("Value: %.2f\n", value->float_value);
    //         break;
    //     // Add more cases as needed for other types
    //     default:
    //         printf("Unhandled value type: %d\n", value->type);
    //         break;
    //     }
    // }
    return child;
}

AST *vist_binop(AST *child)
{
    // printf("hitted");
    // printf("\n%d + %d\n", child->left, child->right->int_value);
    AST *left = vist(child->left);
    AST *right = vist(child->right);

    AST *result = calloc(1, sizeof(struct AST_STRUCT));

    if (!left || !right)
    {
        printf("Error: Left or Right child is NULL\n");
        exit(1);
    }

    // Handle integer operations
    if (left->type == AST_INT && right->type == AST_INT)
    {
        result->type = AST_INT;
        switch (child->type)
        {
        case AST_ADD:
            result->int_value = left->int_value + right->int_value;
            break;
        case AST_SUB:
            result->int_value = left->int_value - right->int_value;
            break;
        case AST_MUL:
            result->int_value = left->int_value * right->int_value;
            break;
        case AST_DIV:
            if (right->int_value == 0)
            {
                printf("Error: Division by zero\n");
                exit(1);
            }
            result->int_value = left->int_value / right->int_value;
            break;
        default:
            printf("Unsupported operation for int types\n");
            exit(1);
        }
    }
    // Handle float operations
    else if (left->type == AST_FLOAT && right->type == AST_FLOAT)
    {
        result->type = AST_FLOAT;
        switch (child->type)
        {
        case AST_ADD:
            result->float_value = left->float_value + right->float_value;
            break;
        case AST_SUB:
            result->float_value = left->float_value - right->float_value;
            break;
        case AST_MUL:
            result->float_value = left->float_value * right->float_value;
            break;
        case AST_DIV:
            if (right->float_value == 0.0)
            {
                printf("Error: Division by zero\n");
                exit(1);
            }
            result->float_value = left->float_value / right->float_value;
            break;
        default:
            printf("Unsupported operation for float types\n");
            exit(1);
        }
    }
    // Handle mixed integer and float operations
    else if ((left->type == AST_INT && right->type == AST_FLOAT) ||
             (left->type == AST_FLOAT && right->type == AST_INT))
    {
        result->type = AST_FLOAT;
        float left_val = (left->type == AST_INT) ? left->int_value : left->float_value;
        float right_val = (right->type == AST_INT) ? right->int_value : right->float_value;

        switch (child->type)
        {
        case AST_ADD:
            result->float_value = left_val + right_val;
            break;
        case AST_SUB:
            result->float_value = left_val - right_val;
            break;
        case AST_MUL:
            result->float_value = left_val * right_val;
            break;
        case AST_DIV:
            if (right_val == 0.0)
            {
                printf("Error: Division by zero\n");
                exit(1);
            }
            result->float_value = left_val / right_val;
            break;
        default:
            printf("Unsupported operation for mixed types\n");
            exit(1);
        }
    }
    else
    {
        printf("Type error: Mismatched types in arithmetic expression: left type %d, right type %d\n", left->type, right->type);
        exit(1);
    }
    // printf("%d", result);
    return result;
}

AST *vist_compund(AST *child)
{
    for (int i = 0; i < child->compound_value_size; i++)
    {
        // printf("COMPOUND: %s", child->compound_value[i]->string_value);
        vist(child->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}