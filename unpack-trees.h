#ifndef UNPACK_TREES_H
#define UNPACK_TREES_H

#define MAX_UNPACK_TREES 8

struct unpack_trees_options;
struct exclude_list;

typedef int (*merge_fn_t)(struct cache_entry **src,
		struct unpack_trees_options *options);

enum unpack_trees_error_types {
	ERROR_WOULD_OVERWRITE = 0,
	ERROR_NOT_UPTODATE_FILE,
	ERROR_NOT_UPTODATE_DIR,
	ERROR_WOULD_LOSE_UNTRACKED_OVERWRITTEN,
	ERROR_WOULD_LOSE_UNTRACKED_REMOVED,
	ERROR_BIND_OVERLAP,
	ERROR_SPARSE_NOT_UPTODATE_FILE,
	ERROR_WOULD_LOSE_ORPHANED_OVERWRITTEN,
	ERROR_WOULD_LOSE_ORPHANED_REMOVED,
	NB_UNPACK_TREES_ERROR_TYPES
};

/*
 * Sets the list of user-friendly error messages to be used by the
 * command "cmd" (either merge or checkout)
 */
void setup_unpack_trees_porcelain(const char **msgs, const char *cmd);

struct rejected_paths_list {
	char *path;
	struct rejected_paths_list *next;
};

struct unpack_trees_options {
	unsigned int reset,
		     merge,
		     update,
		     index_only,
		     nontrivial_merge,
		     trivial_merges_only,
		     verbose_update,
		     aggressive,
		     skip_unmerged,
		     initial_checkout,
		     diff_index_cached,
		     debug_unpack,
		     skip_sparse_checkout,
		     gently,
		     show_all_errors;
	const char *prefix;
	int cache_bottom;
	struct dir_struct *dir;
	merge_fn_t fn;
	const char *msgs[NB_UNPACK_TREES_ERROR_TYPES];
	/*
	 * Store error messages in an array, each case
	 * corresponding to a error message type
	 */
	struct rejected_paths_list *unpack_rejects[NB_UNPACK_TREES_ERROR_TYPES];

	int head_idx;
	int merge_size;

	struct cache_entry *df_conflict_entry;
	void *unpack_data;

	struct index_state *dst_index;
	struct index_state *src_index;
	struct index_state result;

	struct exclude_list *el; /* for internal use */
};

extern int unpack_trees(unsigned n, struct tree_desc *t,
		struct unpack_trees_options *options);

int threeway_merge(struct cache_entry **stages, struct unpack_trees_options *o);
int twoway_merge(struct cache_entry **src, struct unpack_trees_options *o);
int bind_merge(struct cache_entry **src, struct unpack_trees_options *o);
int oneway_merge(struct cache_entry **src, struct unpack_trees_options *o);

#endif
