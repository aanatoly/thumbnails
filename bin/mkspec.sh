#!/bin/bash

mime=$(gdk-pixbuf-query-loaders | grep "image/" | \
              sed -e 's/\s/\n/g' -e 's/"//g' | sort -u)

for m in $mime; do xmime="${xmime}${m};"; done

cat <<EOF
[Thumbnailer Entry]
TryExec=imgth
Exec=imgth -s %s -i %u -o %o
MimeType=${xmime}
EOF
