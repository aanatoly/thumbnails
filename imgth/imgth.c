/* imgth - simple image thumbnailer
 * 
 * Accept image as input and writes its thumbnail to the output.
 * Thumbnail size defaults to 128.
 * For actual image processing uses gdk-pixbuf, to see supported
 * formats, run `gdk-pixbuf-query-loaders`
 *
 * To be compliant with freedesktop spec, caller must compute
 * correct thumbnail path.
 * 
 * Return code: 0 for success, error code otherwise
 */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static gint size = 128;
static gboolean debug = FALSE;
static char *input = NULL;
static char *output = NULL;

static GOptionEntry entries[] = {
    { "size", 's', 0, G_OPTION_ARG_INT, &size,
      "Thumbnail size (default 128)", NULL },
    { "input", 'i', 0, G_OPTION_ARG_STRING, &input,
      "Input image path", NULL },
    { "output", 'o', 0, G_OPTION_ARG_STRING, &output,
      "Output thumbnail path", NULL },
    { "debug", 0, 0, G_OPTION_ARG_NONE, &debug,
      "Debug ouput", NULL },
    { NULL }
};

// 1 - free everything, 0 - free nothing, wait for exit()
// slow_path is usefull in valgrinding the code
static int slow_path = 0;

#define DBG(args...)                            \
    do {                                        \
    if (debug) g_print(args);                   \
    } while (0)
    



static void
mk_thumb(const char *input, char *output, int size)
{
    GdkPixbuf *pix;
    int rc;
    char *path, *uri;
    struct stat statbuf;
    char mtime_str[100];
    GError *error = NULL;
    
    pix = gdk_pixbuf_new_from_file_at_scale(input, size, size, TRUE, &error);
    if (!pix)
        g_error("%s", error->message);

    path = realpath(input, NULL);
    uri = g_filename_to_uri(path, NULL, NULL);
    DBG("name %s\npath %s\nuri %s\n", input, path, uri);
    if (stat(path, &statbuf)) 
        g_error("lstat failed for '%s': %s\n", path, strerror(errno));

    g_snprintf(mtime_str, 100, "%lu", statbuf.st_mtime);
    //gdk_pixbuf_set_option(pix, "tEXt::Thumb::MTime", mtime_str);
    //gdk_pixbuf_set_option(pix, "tEXt::Thumb::URI", uri);
    rc = gdk_pixbuf_save(pix, output, "png", &error,
        "tEXt::Thumb::MTime", mtime_str,
        "tEXt::Thumb::URI", uri,
        NULL);
    if (rc == FALSE)
        g_error("pixbuf save failed %s\n", error->message);

    if (slow_path) {
        g_object_unref(pix);
        g_free(path);
        g_free(uri);
    }
}


    
int main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;
    
    DBG("argc %d\n", argc);    
    context = g_option_context_new ("- create image thumbnail");
    g_option_context_add_main_entries (context, entries, NULL);
    if (!g_option_context_parse (context, &argc, &argv, &error)) 
        g_error("option parsing failed: %s\n", error->message);

    if (!input) 
        g_error("input file is missing");
    if (!output) 
        g_error("output file is missing");
        
    mk_thumb(input, output, size);
    if (slow_path)
        g_option_context_free(context);
    exit(0);
}
