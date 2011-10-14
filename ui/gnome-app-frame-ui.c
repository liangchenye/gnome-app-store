/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Sframe_uite 330,
Boston, MA 02111-1307, USA.

Author: Liang chenye <liangchenye@gmail.com>

*/
#include "st.h"
#define ST_H_INSIDE
#include "st/st-button.h"
#undef ST_H_INSIDE

#include <clutter/clutter.h>
#include "gnome-app-frame-ui.h"
#include "gnome-app-stage.h"

struct _GnomeAppFrameUIPrivate
{
	ClutterActor *box;
	ClutterLayoutManager *layout;
	ClutterActor *layout_box;
};

G_DEFINE_TYPE (GnomeAppFrameUI, gnome_app_frame_ui, CLUTTER_TYPE_GROUP)

static ClutterActor *
create_search_entry (GnomeAppFrameUI *frame_ui)
{
	ClutterActor *entry;
	gchar *text = "Search";

	entry = clutter_text_new_full (NULL, text, CLUTTER_COLOR_LightGray);
	clutter_text_set_editable (CLUTTER_TEXT (entry), TRUE);
	clutter_text_set_selectable (CLUTTER_TEXT (entry), TRUE);
	clutter_text_set_activatable (CLUTTER_TEXT (entry), TRUE);
	clutter_text_set_single_line_mode (CLUTTER_TEXT (entry), TRUE);
	clutter_actor_set_reactive (entry, TRUE);
	clutter_actor_grab_key_focus (entry);

	return entry;
}

static ClutterActor *
create_category_list (GnomeAppFrameUI *frame_ui)
{
	ClutterLayoutManager *layout;
	ClutterActor *layout_box, *actor;
	const GList *list;
	GList *l;
	gchar *name;
	gint col, row;

	layout = clutter_table_layout_new ();
        clutter_table_layout_set_row_spacing (CLUTTER_TABLE_LAYOUT (layout), 10);
	layout_box = clutter_box_new (layout);

	col = row = 0;
	list = (const GList *)gnome_app_get_local_categories ();
	for (l = (GList *)list; l; l = l->next) {
		name = (gchar *)l->data;
		actor = st_button_new_with_label (name);
		clutter_table_layout_pack (CLUTTER_TABLE_LAYOUT (layout), actor, col, row);
		row ++;
	}

	return layout_box;
}

static void
gnome_app_frame_ui_init (GnomeAppFrameUI *frame_ui)
{
	GnomeAppFrameUIPrivate *priv;

	frame_ui->priv = priv = G_TYPE_INSTANCE_GET_PRIVATE (frame_ui,
	                                                 GNOME_APP_TYPE_FRAME_UI,
	                                                 GnomeAppFrameUIPrivate);

	priv->box = clutter_box_new (clutter_box_layout_new ());
	clutter_container_add_actor (CLUTTER_CONTAINER (frame_ui), priv->box);

	priv->layout = clutter_table_layout_new ();
	priv->layout_box = clutter_box_new (priv->layout);
	clutter_container_add_actor (CLUTTER_CONTAINER (priv->box), priv->layout_box);
	clutter_table_layout_set_column_spacing (CLUTTER_TABLE_LAYOUT (priv->layout), 10);
	clutter_table_layout_set_row_spacing (CLUTTER_TABLE_LAYOUT (priv->layout), 10);

	frame_ui->search_entry = create_search_entry (frame_ui);
	frame_ui->stage = gnome_app_stage_new ();
	frame_ui->category_list = create_category_list (frame_ui);

	clutter_actor_set_width (frame_ui->search_entry, 180);
	clutter_actor_set_width (frame_ui->category_list, 180);
	clutter_actor_set_width (frame_ui->stage, 600);
	
	clutter_table_layout_pack (CLUTTER_TABLE_LAYOUT (priv->layout), frame_ui->search_entry, 1, 0);
	clutter_table_layout_pack (CLUTTER_TABLE_LAYOUT (priv->layout), CLUTTER_ACTOR (frame_ui->stage), 0, 1);
	clutter_table_layout_pack (CLUTTER_TABLE_LAYOUT (priv->layout), CLUTTER_ACTOR (frame_ui->category_list), 1, 1);
}

static void
gnome_app_frame_ui_dispose (GObject *object)
{
	G_OBJECT_CLASS (gnome_app_frame_ui_parent_class)->dispose (object);
}

static void
gnome_app_frame_ui_finalize (GObject *object)
{
	GnomeAppFrameUI *frame_ui = GNOME_APP_FRAME_UI (object);
	GnomeAppFrameUIPrivate *priv = frame_ui->priv;

	G_OBJECT_CLASS (gnome_app_frame_ui_parent_class)->finalize (object);
}

static void
gnome_app_frame_ui_class_init (GnomeAppFrameUIClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = gnome_app_frame_ui_dispose;
	object_class->finalize = gnome_app_frame_ui_finalize;
	 
	g_type_class_add_private (object_class, sizeof (GnomeAppFrameUIPrivate));
}

GnomeAppFrameUI *
gnome_app_frame_ui_new (void)
{
	return g_object_new (GNOME_APP_TYPE_FRAME_UI, NULL);
}