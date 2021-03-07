#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = tmp->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    size_t s_len = strlen(s) + 1;
    newh->value = (char *) malloc(sizeof(char) * s_len);
    if (!newh->value) {
        free(newh);
        return false;
    }
    (q->size)++;
    strncpy(newh->value, s, s_len);
    newh->next = q->head;
    q->head = newh;
    if (!q->tail) {
        q->tail = newh;
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        free(newh);
        return false;
    }
    int s_len = strlen(s) + 1;
    newh->value = malloc(sizeof(char) * (s_len + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    (q->size)++;
    strncpy(newh->value, s, s_len);
    if (!q->head) {
        q->head = newh;
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    newh->next = NULL;
    return true;
}


/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }
    if (sp && q->head->value) {
        strncpy(sp, q->head->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;
    free(tmp->value);
    free(tmp);
    (q->size)--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || q->size < 2) {
        return;
    }
    list_ele_t *cur = q->head;
    list_ele_t *tmp = q->head->next;
    q->tail->next = cur;
    while (tmp != q->tail) {
        cur->next = tmp->next;
        tmp->next = q->tail->next;
        q->tail->next = tmp;
        tmp = cur->next;
    }
    q->tail = cur;
    q->tail->next = NULL;
    q->head = tmp;
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

void merge_sort(list_ele_t **head)
{
    if (!(*head) || !((*head)->next)) {
        return;
    }

    list_ele_t *left = *head;           // the pointor of left list
    list_ele_t *right = (*head)->next;  // the pointor of right list

    // split list in to 2 list
    while (right && right->next) {
        left = left->next;
        right = right->next->next;
    }
    right = left->next;
    left->next = NULL;
    left = *head;

    merge_sort(&left);   // the left linked list
    merge_sort(&right);  // the right linked list

    // sorting
    *head = NULL;
    list_ele_t **tmp = head;

    while (right && left) {
        if (strcmp(right->value, left->value) > 0) {  // right > left
            *tmp = left;                              // get left element out
            left = left->next;
        } else {
            *tmp = right;
            right = right->next;
        }
        tmp = &((*tmp)->next);
    }
    if (right) {
        *tmp = right;
    } else {
        *tmp = left;
    }
}
void q_sort(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }

    merge_sort(&q->head);  // q->head = merge_sort(q->head);
    // get tail
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
    return;
}
