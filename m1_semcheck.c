#include <stdio.h>
#include <stdlib.h>
#include "m1_semcheck.h"
#include "m1_ast.h"

static data_type
check_object(M1_compiler *comp, m1_object *obj) {
	switch (obj->type) {
		case OBJECT_MAIN:
		
		case OBJECT_FIELD:
		case OBJECT_INDEX:
		case OBJECT_DEREF:
			
/*		case OBJECT_SCOPE:*/
		default:
			exit(EXIT_FAILURE);
	}	
}

static data_type
check_expr(M1_compiler *comp, m1_expression *expr) {
	data_type t;
	switch (expr->type) {
		case EXPR_NUMBER:
			return TYPE_NUM;
		case EXPR_INT:
			return TYPE_INT;
		case EXPR_BINARY: 
		{
			data_type ltype = check_expr(comp, expr->expr.b->left);
			data_type rtype = check_expr(comp, expr->expr.b->right);
			
			if (ltype != rtype) {
				fprintf(stderr, "Incompatible types in binary expression\n");
				++comp->errors;
			}			
			return ltype;
		}
		case EXPR_UNARY:
    	case EXPR_FUNCALL:
    	case EXPR_ASSIGN:
    	case EXPR_IF:
    	case EXPR_WHILE:
    	case EXPR_DOWHILE:
    	case EXPR_FOR:
    	case EXPR_RETURN:
    	case EXPR_NULL:
    	case EXPR_DEREF:
    	case EXPR_ADDRESS:
		case EXPR_OBJECT:
			return check_object(comp, expr->expr.t);
    	case EXPR_BREAK:
    		break;
	    case EXPR_STRING:
	    	return TYPE_STRING;
	    	
		case EXPR_CONSTDECL:
   		case EXPR_VARDECL:
		case EXPR_M0BLOCK:
    	case EXPR_PRINT:
    		break;
		default:
			break;	
	}
	return t;
}


static data_type
check_chunk(M1_compiler *comp, m1_chunk *chunk) {
	data_type type;
	m1_expression *iter = chunk->block;
	
	while (iter != NULL) {
		type = check_expr(comp, iter);
		if (iter->type == EXPR_RETURN) {
			if (chunk->rettype != type) {
				fprintf(stderr, "Return type of function '%s' does not match type of returned expression\n", chunk->name);
				++comp->errors;	
			}
		}
		iter = iter->next;	
	}

	return chunk->rettype;
}

void 
check(M1_compiler *comp, m1_chunk *ast) {
	m1_chunk *iter = ast;
	
	while (iter != NULL) {
		(void)check_chunk(comp, iter);
		iter = iter->next;	
	}	
}