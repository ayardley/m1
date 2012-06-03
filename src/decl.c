#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "decl.h"
#include "ast.h"


/*

Find the declaration for type <typename>.

*/
m1_decl *
type_find_def(M1_compiler *comp, char *typename) {
    m1_decl *iter;
    
    assert(comp != NULL);

    iter = comp->declarations;
    
    while (iter != NULL) {
        assert(iter->name != NULL);
        assert(typename != NULL);
        
        if (strcmp(iter->name, typename) == 0) { /* found! */
            fprintf(stderr, "Found type %s\n", typename);
            return iter;
        }
        iter = iter->next;    
    }
    
    return NULL;    
}




/*

Enter a new struct declaration that goes by name <structname>.

*/
m1_decl *
type_enter_struct(M1_compiler *comp, char *structname, struct m1_struct *structdef) {
    m1_decl *decl = (m1_decl *)calloc(1, sizeof(m1_decl));    
    
    assert(comp != NULL);
    
    fprintf(stderr, "entering type [%s]\n", structname);
    if (decl == NULL) {
        fprintf(stderr, "cant alloc mem for decl\n");
        exit(EXIT_FAILURE);   
    }
    
    decl->name = structname;
    decl->d.s  = structdef;
    decl->type = DECL_STRUCT;
    
    /* link in list of declarations */
    decl->next = comp->declarations;
    comp->declarations = decl;
    
    return decl;
}

/* 

Interface for declaring basic types. 

*/
m1_decl *
type_enter_type(M1_compiler *comp, char *typename, m1_decl_type type, unsigned size) {
    m1_decl *decl = (m1_decl *)calloc(1, sizeof(m1_decl));
    
    decl->name   = typename;
    decl->type   = type;
    decl->d.size = size;
    
    decl->next = comp->declarations;
    comp->declarations = decl;
    
    return decl;        
}
