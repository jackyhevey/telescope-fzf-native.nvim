#ifndef _FZF_H_
#define _FZF_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  int16_t *data;
  size_t size;
  size_t cap;
  bool allocated;
} i16_t;

typedef struct {
  int32_t *data;
  size_t size;
  size_t cap;
  bool allocated;
} i32_t;

typedef struct {
  unsigned char *data;
  size_t len;
  size_t ulen;
} string_t;

string_t init_string(char *str);

typedef struct {
  size_t *data;
  size_t size;
  size_t cap;
} position_t;

typedef struct {
  int32_t start;
  int32_t end;
  int32_t score;

  position_t *pos;
} result_t;

typedef struct {
  i16_t I16;
  i32_t I32;
} slab_t;

typedef enum {
  term_fuzzy = 0,
  term_exact,
  term_prefix,
  term_suffix,
  term_equal
} alg_types;

typedef enum { case_smart = 0, case_ignore, case_respect } case_types;

typedef result_t (*algorithm_t)(bool, bool, string_t *, string_t *, bool,
                                slab_t *);
typedef struct {
  alg_types typ;
  algorithm_t alg;
  bool inv;
  char *ptr;
  string_t text;
  bool case_sensitive;
} term_t;

typedef struct {
  term_t *ptr;
  size_t size;
  size_t cap;
} term_set_t;

typedef struct {
  term_set_t **ptr;
  size_t size;
  size_t cap;
  bool only_inv;
} pattern_t;

result_t fuzzy_match_v2(bool case_sensitive, bool normalize, string_t *text,
                        string_t *pattern, bool with_pos, slab_t *slab);
result_t fuzzy_match_v1(bool case_sensitive, bool normalize, string_t *text,
                        string_t *pattern, bool with_pos, slab_t *slab);
result_t exact_match_naive(bool case_sensitive, bool normalize, string_t *text,
                           string_t *pattern, bool with_pos, slab_t *slab);
result_t prefix_match(bool case_sensitive, bool normalize, string_t *text,
                      string_t *pattern, bool with_pos, slab_t *slab);
result_t suffix_match(bool case_sensitive, bool normalize, string_t *text,
                      string_t *pattern, bool with_pos, slab_t *slab);
result_t equal_match(bool case_sensitive, bool normalize, string_t *text,
                     string_t *pattern, bool with_pos, slab_t *slab);

/* interface */
pattern_t *parse_pattern(case_types case_mode, bool normalize, char *pattern);
void free_pattern(pattern_t *pattern);

position_t *get_positions(char *text, pattern_t *pattern, slab_t *slab);
void free_positions(position_t *pos);
void iter_positions(position_t *pos, void (*handle_position)(size_t pos));
int32_t get_score(char *text, pattern_t *pattern, slab_t *slab);

slab_t *make_slab(size_t size_16, size_t size_32);
slab_t *make_default_slab(void);
void free_slab(slab_t *slab);

#endif // _fzf_H_
