/** @mainpage DocComment: 
 * Introduction, basic syntax/features */

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include <glib.h>
#include <log4c.h>

#include <yajl/yajl_tree.h>
#include <json-c/json.h>
#include <yaml.h>

#include "intro_c/util.h"
#include "intro_c/person.h"
#include "intro.h"
#include "intro_c/classic.h"
#include "intro_c/sequenceops.h"

#define OVECCOUNT 30

//#define PI 3.14
float const PI = 3.14f;

typedef char unsigned uchar_t;

enum ConstItems {ZERO, NUMZ = 26};

enum Kind {FLOAT, DOUBLE, DOUBLEL, INTSHORT, UINTLONG};

struct opts_record {char name[32]; int num; bool is_expt2;
};

struct user {char name[32]; int num; time_t time_in;
};

struct u_var {
    int kind;
    union {
        // optional int specifiers: [unsigned|signed] [short|long|long long]
        short signed sh; long unsigned ul;
        
        float f;
        // optional double specifiers: [long]
        double d; double long m;
    } val;
};

static log4c_category_t *root;

static void run_intro(const char *progname, char *rsrc_path, 
        struct opts_record *opts) {
    time_t time_in = time(NULL);
    srand(time_in);
    struct timespec t1, t2;
    
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &t1))  // to use, link w/ -lrt
        fprintf(stderr, "clock_gettime error for t1.\n");
    
    // basic datatypes
    bool is_done = false;
    int num_i = 0, arr_len = ZERO;
    const int len_repeat = 80;
    float time_diff = 0.0f, delay_usecs = 2.5e6;
    char ch = '\0';
    
    // pointers
    char path_buf[128];
    snprintf(path_buf, sizeof(path_buf) - 1, "%s/greet.txt", rsrc_path);
    char *greetBuf = NULL, *greet_path = path_buf;
    int *num_ptr = &num_i;
    FILE *fOut = stdout;
    
    // string & arrays
    char *noname = "World", dateBuf[64];
    const char buf_repeat[len_repeat];
    int numArr[] = {11, 013, 0xb, 11};  // {bin, oct, hex, dec}
    
    // composites
    struct u_var u_var1;
    struct user user1 = {.num = opts->num, .time_in = time_in};
    PERSON person1 = construct_person("I.M. Computer", 32);
    
    struct user *user_ptr = &user1;
    
    arr_len = sizeof(numArr) / sizeof(numArr[0]);
    
    for (int i = 0; arr_len > i; ++i)
        num_i += numArr[i];
    assert((arr_len * numArr[0]) == *num_ptr);
    
    ch = delay_char(delay_usecs);
    
    if (0 == user_ptr->num)
        user_ptr->num = (rand() % 17) + 2;
    
    strncpy(user1.name, opts->name, 1 + strlen(opts->name));
    
    //regex_t regex;
    //compile_regex(&regex, "^quit$", REG_EXTENDED|REG_NEWLINE|REG_ICASE);
    int ovector[OVECCOUNT];
    pcre *re = compile_pcre("^quit$", PCRE_CASELESS);//("(?i)^quit$", 0);
    
    //int rc = match_regex(&regex, user1.name);
    int rc = match_pcre(re, ovector, user1.name);
    
    printf("%s match: %s to %s\n", (0 > rc) ? "Does not" : "Good", user1.name, 
        "\"^quit$\"");
    
    greeting(greet_path, user1.name, &greetBuf);
    strftime(dateBuf, sizeof(dateBuf), "%c %z %Z", localtime(&user1.time_in));
    printf("%s\n%s!\n", dateBuf, greetBuf);
    
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &t2)) 
        fprintf(stderr, "clock_gettime error for t2.\n");
    time_diff = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1.0e9;
    printf("(program %s) Took %.1f seconds.\n", progname, time_diff);
    printf("%s\n", string_replicate(40, "#", len_repeat, buf_repeat));
    
    do {
        u_var1.kind = INTSHORT; u_var1.val.sh = -1;
        u_var1.kind = UINTLONG; u_var1.val.ul = 1UL;
        u_var1.kind = FLOAT; u_var1.val.f = 0.0f;
        u_var1.kind = DOUBLE; u_var1.val.d = 100.0;
        u_var1.kind = DOUBLEL; u_var1.val.m = 1.0e6L;
    } while (is_done);
    
    int n0 = 0, n1 = 1, n2 = 2, n3 = 3, n4 = 4;
    const void *ptr1_ints[] = {&n0, &n1, &n2, &n3, &n4};
    int len_ints = sizeof(ptr1_ints) / sizeof(ptr1_ints[0]);
    char *result = calloc(1, sizeof(char));
    
    int arr1[] = {9, 9, 9, 9}, arr2[] = {0, 1, 2, 3, 4};
    GArray *gint_arr = g_array_new(FALSE, FALSE, sizeof(gint));
    g_array_append_vals(gint_arr, arr1, 4);
    
    if (opts->is_expt2) {
        fprintf(fOut, "expt_i(%.1f, %.1f) = %.1f\n", 2.0f, (float)user1.num, 
            expt_i(2.0f, (float)user1.num));
        
        void **ptr2_tmps = copy_of(len_ints, ptr1_ints);
        mkstring_ptrarray(intptr_el_to_str, len_ints, (const void**)ptr2_tmps, 
            &result);
        printf("reverse_lp(len_ints(%i), ptr2_tmps(%s)): ", len_ints, result);
        free(result);
        result = calloc(1, sizeof(char));
        reverse_lp(len_ints, ptr2_tmps);
        mkstring_ptrarray(intptr_el_to_str, len_ints, (const void**)ptr2_tmps, 
            &result);
        printf("%s\n", result);
        free(ptr2_tmps);
        free(result);
        result = calloc(1, sizeof(char));
        
        g_array_append_vals(gint_arr, arr2, 5);
        qsort(gint_arr->data, gint_arr->len, sizeof(int), int_cmp);
        mkstring_ints(gint_arr->len, (const int*)gint_arr->data, &result);
        printf("qsort(append(arr1, arr2), len_arr1 + len_arr2, sizeof(int), "
            "int_cmp): %s\n", result);
    } else {
        fprintf(fOut, "fact_i(%i) = %li\n", user1.num, fact_i(user1.num));
        
        int el = 3;
        mkstring_ptrarray(intptr_el_to_str, len_ints, ptr1_ints, &result);
        printf("index_of_lp(&el(%i), len_ints(%i), ptr1_ints(%s), int_cmp): "
            "%i\n", el, len_ints, result, index_of_lp(&el, len_ints, 
            ptr1_ints, int_cmp));
        free(result);
        result = calloc(1, sizeof(char));
        
        g_array_append_vals(gint_arr, arr2, 5);
        mkstring_ints(gint_arr->len, (const int*)gint_arr->data, &result);
        printf("append(arr1, arr2): %s\n", result);
    }
    g_array_free(gint_arr, TRUE);
    free(result);
    
    printf("%s\n", string_replicate(40, "#", len_repeat, buf_repeat));
    char buf[80];
    person_toString(person1, 80, buf);
    printf("person_toString(person1, 80, buf): %s\n", buf);
    person_setAge(person1, 33);
    printf("person_setAge(person1, %i): \n", 33);
    person_toString(person1, 80, buf);
    printf("person_toString(person1, 80, buf): %s\n", buf);
    
    printf("%s\n", string_replicate(40, "#", len_repeat, buf_repeat));

    fflush(stdout);
    
    assert(NULL != re);
    //regfree(&regex);
    pcre_free(re);
    
    destruct_person(person1);
    free(greetBuf);
}


static void print_usage(const char *str) {
    fprintf(stderr, "Usage: %s [-h][-2][-u NAME][-n NUM]\n", str);
    exit(EXIT_FAILURE);
}

void parse_cmdopts(struct opts_record *opts, int argc, char **argv) {
    int8_t opt_ch;
    const char *opts_str = "u:n:2h";
	log4c_category_log(root, LOG4C_PRIORITY_INFO, "parse_cmdopts()");
    //if (2 > argc)
    //    print_usage(argv[0]);
    while (-1 != (opt_ch = getopt(argc, argv, opts_str))) {
        switch (opt_ch) {
            case 'u': strncpy(opts->name, optarg, 1 + strlen(optarg));
                break;
            case 'n': chk_strtol(&opts->num, optarg, NULL, 10);
                break;
            case '2': opts->is_expt2 = true;
                break;
            default: print_usage(argv[0]);
        }
    }
    if (argc > optind)
        fprintf(stderr, "Unexpected argument after options\n");
}

static int chk_inidata(GKeyFile *cfg_ini, const char *buf_ini, 
		GKeyFileFlags flags) {
    GError *error = NULL;
    
    if (!g_key_file_load_from_data(cfg_ini, buf_ini, strlen(buf_ini), flags, 
			&error)) {
		g_error("%s", error->message);
		g_error_free(error);
		return -1;
	}
    return 0;
}

/** DocComment: <p>
 * Brief description. <p>
 * @param argc - number of command-line arguments
 * @param argv - array of command-line arguments */
int main(int argc, char **argv) {
    struct opts_record opts;
    FILE *fIn;
    char rc_buf[256],
		*rsrc_path = getenv("RSRC_PATH") ? getenv("RSRC_PATH") : "resources";
    
    log4c_init();
    // default precedence: ${LOG4C_RCPATH}/log4crc, ${HOME}/.log4crc, ./log4crc
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/log4crc.xml", rsrc_path);
    if (log4c_load(rc_buf))
    	fprintf(stderr, "Failed loading log4c config file($RSRC_PATH=%s): %s\n",
			rsrc_path, rc_buf);
    root = log4c_category_get("root");
    
    strncpy(opts.name, "World", 1 + strlen("World"));
    parse_cmdopts(&opts, argc, argv);
    
    char buf_ini[1024], *row_ini[3] = {"????\n", "???", "???"};
    GKeyFile *cfg_ini = g_key_file_new();
    GError *error = NULL;
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/prac.conf", rsrc_path);
    if (NULL == (fIn = fopen(rc_buf, "r")))
		fprintf(stderr, "Failed loading config file($RSRC_PATH=%s): %s\n",
			rsrc_path, rc_buf);
	else {
		fread(buf_ini, sizeof(buf_ini[0]), sizeof(buf_ini), fIn);
		
		if (chk_inidata(cfg_ini, buf_ini, G_KEY_FILE_KEEP_COMMENTS)) {
			g_key_file_free(cfg_ini);
		} else {
			row_ini[0] = inicfg_to_str(cfg_ini);
			row_ini[1] = g_key_file_get_string(cfg_ini, "default", "domain", 
				&error);
			row_ini[2] = g_key_file_get_string(cfg_ini, "user1", "name",
				&error);
		}
    }
    
    char buf_json[1024], *row_json[3] = {"????\n", "???", "???"};
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/prac.json", rsrc_path);
    if (NULL == (fIn = fopen(rc_buf, "r")))
		fprintf(stderr, "Failed loading config file($RSRC_PATH=%s): %s\n",
			rsrc_path, rc_buf);
	else {
		fread(buf_json, sizeof(buf_json[0]), sizeof(buf_json), fIn);
		
		if (1) {
			json_object *jsonobj1 = NULL, *domainobj = NULL, *user1obj = NULL,
				*user1Nameobj = NULL;
			if (NULL == (jsonobj1 = json_tokener_parse(buf_json)))
				fprintf(stderr, "Failure: JSON object is NULL\n");
			else
				row_json[0] = buf_json;
			if (!json_object_object_get_ex(jsonobj1, "domain", &domainobj))
				fprintf(stderr, "Failure: path domain is missing\n");
			else
				row_json[1] = (char*)json_object_get_string(domainobj);
			if (!json_object_object_get_ex(jsonobj1, "user1", &user1obj))
				fprintf(stderr, "Failure: path user1:name is missing\n");
			else {
				json_object_object_get_ex(user1obj, "name", &user1Nameobj);
				row_json[2] = (char*)json_object_get_string(user1Nameobj);
			}
		} else {
			char errbuf[512];
			yajl_val jsonnode = NULL, v_domain = NULL, v_user1Name = NULL;
			const char *path0[] = {"domain", NULL},
				*path1[] = {"user1", "name", NULL};
				
			if (NULL == (jsonnode = yajl_tree_parse(buf_json, errbuf, 
					sizeof(errbuf))))
				fprintf(stderr, "Failure: JSON node is NULL\n");
			else 
				row_json[0] = buf_json;
			if (NULL == (v_domain = yajl_tree_get(jsonnode, path0,
					yajl_t_string)))
				fprintf(stderr, "Failure: path domain is missing\n");
			else
				row_json[1] = YAJL_GET_STRING(v_domain);
			if (NULL == (v_user1Name = yajl_tree_get(jsonnode, path1, 
					yajl_t_string)))
				fprintf(stderr, "Failure: path user1:name is missing\n");
			else
				row_json[2] = YAJL_GET_STRING(v_user1Name);
			yajl_tree_free(jsonnode);
		}
	}
    
    char buf_yaml[1024], *row_yaml[3] = {"????\n", "???", "???"};
    snprintf(rc_buf, sizeof(rc_buf) - 1, "%s/prac.yaml", rsrc_path);
    if (NULL == (fIn = fopen(rc_buf, "r")))
		fprintf(stderr, "Failed loading config file($RSRC_PATH=%s): %s\n",
			rsrc_path, rc_buf);
	else {
		fread(buf_yaml, sizeof(buf_yaml[0]), sizeof(buf_yaml), fIn);
		yaml_parser_t yamlparser;
		yaml_token_t token_yaml;
		if (!yaml_parser_initialize(&yamlparser))
			fprintf(stderr, "Failure: Yaml parser initialization failed\n");
		else {
			yaml_parser_set_input_string(&yamlparser, buf_yaml,
				sizeof(buf_yaml) / sizeof(buf_yaml[0]));
			char *name_block = "default", *tk, *keyX, *valX, **data_ptr;
			char buf_tmp[256];
			int is_value = 0;
			GData *datalist = NULL;
			g_datalist_init(&datalist);
			
			do {
				yaml_parser_scan(&yamlparser, &token_yaml);
				
				switch(token_yaml.type) {
					case YAML_KEY_TOKEN:
						is_value = 0; break;
					case YAML_VALUE_TOKEN:
						is_value = 1; break;
					case YAML_SCALAR_TOKEN:
						tk = token_yaml.data.scalar.value;
						if (0 == is_value) {
							keyX = strdup(tk);
						} else {
							snprintf(buf_tmp, sizeof(buf_tmp), "%s:%s",
								name_block, keyX);
							g_datalist_set_data(datalist, strdup(buf_tmp),
								strdup(tk));
						}
						break;
					case YAML_BLOCK_SEQUENCE_START_TOKEN:
						tk = token_yaml.data.scalar.value;
						name_block = strdup(tk);
						break;
					default:
						break;
				}
				if (NULL == token_yaml.type || YAML_STREAM_END_TOKEN != token_yaml.type)
					yaml_token_delete(&token_yaml);
			} while (NULL != token_yaml.type && YAML_STREAM_END_TOKEN != token_yaml.type);
			yaml_token_delete(&token_yaml);
			
			row_yaml[0] = buf_yaml;
			row_yaml[1] = g_datalist_get_data(datalist, "default:domain");
			row_yaml[2] = g_datalist_get_data(datalist, "user1:name");
			
			yaml_parser_delete(&yamlparser);
		}
	}
    
    const char *arr_rows[][3] = { {row_ini[0], row_ini[1], row_ini[2]},
		{row_json[0], row_json[1], row_json[2]},
		{row_yaml[0], row_yaml[1], row_yaml[2]} };
    int len_arr_rows = sizeof(arr_rows) / sizeof(arr_rows[0]);
    
    //gchar *res_str = g_key_file_get_start_group(cfg_ini);
    //fprintf(stderr, "GKeyFile config start group: %s\n", res_str);
    //g_free(res_str);
    for (int i = 0; len_arr_rows > i; ++i) {
		printf("config: %s", arr_rows[i][0]);
		printf("domain: %s\n", arr_rows[i][1]);
		printf("user1Name: %s\n\n", arr_rows[i][2]);
	}
    
    if (NULL != error)
		g_error_free(error);
    g_key_file_free(cfg_ini);
    
    run_intro(argv[0], rsrc_path, &opts);
    
    log4c_category_log(root, LOG4C_PRIORITY_DEBUG, "exiting main()");
	log4c_fini();
	exit(EXIT_SUCCESS);
} // end main

