#include <stdio.h>

#include "gnome-app-install.h"

gboolean
gnome_app_install (gchar *pkgname)
{
	/* FIXME: TODO */
	return TRUE;

        gchar *cmd;
        cmd = g_strdup_printf ("/sbin/YaST2 -i %s &", pkgname);
	printf ("run cmd [%s]\n", cmd);
        system (cmd);
        g_free (cmd);

	return TRUE;
}

