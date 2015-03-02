/* simple image thumbnailer */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static gint size = 128;
static gboolean debug = FALSE;
static char *input = NULL;
static char *output = NULL;

// 1 - free everything, 0 - free nothing, wait for exit()
static int slow_path = 0;
static GOptionEntry entries[] = {
    { "size", 's', 0, G_OPTION_ARG_INT, &size,
      "Thumbnail size (default 128)", NULL },
    { "input", 'i', 0, G_OPTION_ARG_STRING, &input,
      "Input image path", NULL },
    { "output", 'o', 0, G_OPTION_ARG_STRING, &output,
      "Output thumbnail path, if omited, it is calculated "
      "according to the freedesktop spec", NULL },
    { "debug", 0, 0, G_OPTION_ARG_NONE, &debug,
      "Debug ouput", NULL },
    { NULL }
};

#define DBG(args...)                            \
    do {                                        \
    if (debug) g_print(args);                   \
    } while (0)
    

static char *
get_dstdir(int size)
{
    char *name = NULL, *dstdir;
    
    if (size == 128)
        name = "normal";
    else if (size == 256)
        name = "large";
    else
        g_error("Can't decide on ~/.thumbnails subdir: non standard thumb size %d", size);
    
    dstdir = g_build_filename(g_get_home_dir(), ".thumbnails", name, NULL);
    if (g_mkdir_with_parents(dstdir, 0700)) 
        g_error("can't mkdir %s\n", dstdir);
    return dstdir;
}



static void
mk_thumb(const char *input, char *output, int size)
{
    GdkPixbuf *pix;
    int rc;
    char *cwd, *path, *uri;
    struct stat statbuf;
    char mtime_str[100];
    GError *error = NULL;
    
    pix = gdk_pixbuf_new_from_file_at_scale(input, size, size, TRUE, &error);
    if (!pix)
        g_error("%s", error->message);

    cwd = g_get_current_dir();
    path = g_build_filename(cwd, input, NULL);
    uri = g_filename_to_uri(path, NULL, NULL);
    DBG("cwd %s\nname %s\npath %s\nuri %s\n", cwd, input, path, uri);
    if (lstat(path, &statbuf)) 
        g_error("lstat failed for %s\n", path);

    g_snprintf(mtime_str, 100, "%lu", statbuf.st_mtime);
    //gdk_pixbuf_set_option(pix, "tEXt::Thumb::MTime", mtime_str);
    //gdk_pixbuf_set_option(pix, "tEXt::Thumb::URI", uri);
    if (!output) {
        gchar *csum = g_compute_checksum_for_string(G_CHECKSUM_MD5, uri, -1);
        gchar *dst = get_dstdir(size);
        
        if (slow_path) g_free(path);
        path = g_strdup_printf("%s%s%s.png", dst, G_DIR_SEPARATOR_S, csum);
        DBG("th path %s\n", path);
        if (slow_path) g_free(csum);
        if (slow_path) g_free(dst);
        rc = gdk_pixbuf_save(pix, path, "png", &error,
            "tEXt::Thumb::MTime", mtime_str,
            "tEXt::Thumb::URI", uri,
            NULL);
    } else {
        rc = gdk_pixbuf_save(pix, output, "png", &error,
            "tEXt::Thumb::MTime", mtime_str,
            "tEXt::Thumb::URI", uri,
            NULL);
    }
    if (rc == FALSE)
        g_error("pixbuf save failed %s\n", error->message);
    if (!output)
        g_print("%s : %s\n", input, path);
        
    if (slow_path) {
        g_object_unref(pix);
        g_free(cwd);
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
        
    mk_thumb(input, output, size);
    exit(0);
}
