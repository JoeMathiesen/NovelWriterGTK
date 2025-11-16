#include <gtk/gtk.h>

void end_program(GtkWidget *wid, gpointer ptr)
{
  gtk_main_quit();
}

void apply_css(GtkWidget *wid, GtkStyleProvider *provider)
{
  gtk_style_context_add_provider(gtk_widget_get_style_context(wid),
				 provider,
				 GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void file_selected (GtkFileChooserButton *btn, gpointer ptr)
{
  printf("%s selected\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (btn)));
}

static void open(GtkWidget *dialog, gpointer ptr)
{
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
					NULL,
					action,
					"_Cancel",
					GTK_RESPONSE_CANCEL,
					"_Open",
					GTK_RESPONSE_ACCEPT,
					NULL);
  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      g_free (filename);
    }
  gtk_widget_destroy(dialog);
}

static void save_file (GtkWidget *btn, gpointer ptr)
{
	GtkWidget *sch = gtk_file_chooser_dialog_new("Save file",
		GTK_WINDOW (ptr), GTK_FILE_CHOOSER_ACTION_SAVE,
		"Cancel", 0, "OK", 1, NULL);
	if (gtk_dialog_run (GTK_DIALOG (sch)) == 1)
	{
		printf("%s selected\n", gtk_file_chooser_get_filename
			(GTK_FILE_CHOOSER (sch)));
	}
	gtk_widget_destroy(sch);
}

static void saveas_file (GtkWidget *btn, gpointer ptr)
{
	GtkWidget *sch = gtk_file_chooser_dialog_new("Save file as",
		GTK_WINDOW (ptr), GTK_FILE_CHOOSER_ACTION_SAVE,
		"Cancel", 0, "OK", 1, NULL);
	if (gtk_dialog_run (GTK_DIALOG (sch)) == 1)
	{
		printf("%s selected\n", gtk_file_chooser_get_filename
			(GTK_FILE_CHOOSER (sch)));
	}
	gtk_widget_destroy(sch);
}

void main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);

  // Variables
  int pos = 0;

  // Widgets
  GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *grd = gtk_grid_new();
  GtkWidget *btn = gtk_button_new_with_label("Close Window");
  GtkWidget *lbl = gtk_label_new("Mathiesen Novel Writer");
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *split_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *typewriter_window = gtk_scrolled_window_new(NULL, NULL);
  GtkWidget *typewriter;
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  PangoFontDescription *font_desc;
  GdkRGBA rgba;
  GtkTextTag *tag;
  GtkCssProvider *provider;
  GtkStyleContext *context;

  typewriter = gtk_text_view_new();
  gtk_widget_set_name(right_box, "rightBox");
  gtk_widget_set_name(typewriter, "myTypeWriter");

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(typewriter));

  gtk_text_buffer_set_text(buffer, "Start your novel", -1);

  provider = gtk_css_provider_new();
  gtk_css_provider_load_from_data (provider,
				   "#myTypeWriter {"
				   " background-color: rgb(255,255,255);"
				   " font: 18px 'Sans';"
				   " border: 2px solid #00ffff;"
				   " padding: 8px;"
				   " color: rgb(0,0,0);"
				   " caret-color: rgb(0,0,0);"
				   "}"
				   "#myTypeWriter text {"
				   " background-color: white;"
				   " color: #000000;"
				   " caret-color: rgb(0,0,0);"
				   "}"
				   "box * {"
				   " background:none;"
				   "}"
				   ,
				   -1,
				   NULL);
  context = gtk_widget_get_style_context(typewriter);
  gtk_style_context_add_provider (context,
				  GTK_STYLE_PROVIDER (provider),
				  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (typewriter), 30);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (typewriter), 30);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (typewriter), GTK_WRAP_WORD);

  // Menu Bar
  GtkWidget *mbar = gtk_menu_bar_new();
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *file_mi = gtk_menu_item_new_with_label("File");
  GtkWidget *f_menu = gtk_menu_new();
  GtkWidget *edit_mi = gtk_menu_item_new_with_label("Edit");
  GtkWidget *e_menu = gtk_menu_new();
  GtkWidget *view_mi = gtk_menu_item_new_with_label("View");
  GtkWidget *v_menu = gtk_menu_new();
  GtkWidget *tools_mi = gtk_menu_item_new_with_label("Tools");
  GtkWidget *t_menu = gtk_menu_new();
  // File Menu Bar Items
  GtkWidget *new_mi = gtk_menu_item_new_with_label("New");
  GtkWidget *open_mi = gtk_menu_item_new_with_label("Open");
  GtkWidget *save_mi = gtk_menu_item_new_with_label("Save");
  GtkWidget *saveas_mi = gtk_menu_item_new_with_label("Save As");
  GtkWidget *quit_mi = gtk_menu_item_new_with_label("Quit");
  // Edit Menu Bar Items
  GtkWidget *copy_mi = gtk_menu_item_new_with_label("Copy");
  GtkWidget *cut_mi = gtk_menu_item_new_with_label("Cut");
  GtkWidget *paste_mi = gtk_menu_item_new_with_label("Paste");
  // View Menu Bar Items
  GtkWidget *fontsel_mi = gtk_menu_item_new_with_label("Font");
  GtkWidget *fontsize_mi = gtk_menu_item_new_with_label("Font Size");
  // Tools Menu Bar Items
  GtkWidget *charbuilder_mi = gtk_menu_item_new_with_label("Character Builder");
  GtkWidget *scenebuilder_mi = gtk_menu_item_new_with_label("Scene Builder");

  // Main Menu Items
  gtk_menu_shell_append(GTK_MENU_SHELL (mbar), file_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (mbar), edit_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (mbar), view_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (mbar), tools_mi);
  // Sub Menus
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (file_mi), f_menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (edit_mi), e_menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (view_mi), v_menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (tools_mi), t_menu);
  // File Sub Menu Items
  gtk_menu_shell_append(GTK_MENU_SHELL (f_menu), new_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (f_menu), open_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (f_menu), save_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (f_menu), saveas_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (f_menu), quit_mi);
  // Edit Sub Menu Items
  gtk_menu_shell_append(GTK_MENU_SHELL (e_menu), copy_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (e_menu), cut_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (e_menu), paste_mi);
  // view Sub Menu Items
  gtk_menu_shell_append(GTK_MENU_SHELL (v_menu), fontsel_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (v_menu), fontsize_mi);
  // Tools Sub Menu Items
  gtk_menu_shell_append(GTK_MENU_SHELL (t_menu), charbuilder_mi);
  gtk_menu_shell_append(GTK_MENU_SHELL (t_menu), scenebuilder_mi);

  // File Sub Menu Signals
  g_signal_connect (new_mi, "activate", G_CALLBACK(open), NULL);
  g_signal_connect (open_mi, "activate", G_CALLBACK(open), NULL);
  g_signal_connect (save_mi, "activate", G_CALLBACK(save_file), NULL);
  g_signal_connect (saveas_mi, "activate", G_CALLBACK(saveas_file), NULL);
  g_signal_connect (quit_mi, "activate", G_CALLBACK(end_program),NULL);
  // Edit Sub Menu Signals
  g_signal_connect (copy_mi, "activate", G_CALLBACK(end_program), NULL);
  g_signal_connect (cut_mi, "activate", G_CALLBACK(end_program), NULL);
  g_signal_connect (paste_mi, "activate", G_CALLBACK(end_program), NULL);
  // View Sub Menu Signals
  g_signal_connect (fontsel_mi, "activate", G_CALLBACK(end_program), NULL);
  g_signal_connect (fontsize_mi, "activate", G_CALLBACK(end_program), NULL);
  // Tools Sub Menu Signals
  g_signal_connect (charbuilder_mi, "activate", G_CALLBACK(end_program), NULL);
  g_signal_connect (charbuilder_mi, "activate", G_CALLBACK(end_program), NULL);

  // Side Panel Tree View
  GtkListStore *ls = gtk_list_store_new (2, G_TYPE_STRING, GDK_TYPE_PIXBUF);
  GdkPixbuf *icon = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), "dialog-ok-apply", 32, 0, NULL);
  gtk_list_store_insert_with_values (ls, NULL, pos++, 0, "Chapter 1", 1, icon, -1);
  gtk_list_store_insert_with_values (ls, NULL, pos++, 0, "Chapter 2", 1, icon, -1);
  gtk_list_store_insert_with_values (ls, NULL, pos++, 0, "Chapter 3", 1, icon, -1);

  GtkWidget *tv = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ls));

  GtkCellRenderer *prend = gtk_cell_renderer_pixbuf_new();
  GtkCellRenderer *trend = gtk_cell_renderer_text_new();

  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Icon", prend, "pixbuf", 1, NULL);
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tv), -1, "Option", trend, "text", 0, NULL);

  // Signals
  g_signal_connect (btn, "clicked", G_CALLBACK(end_program), NULL);
  g_signal_connect (win, "delete_event", G_CALLBACK(end_program), NULL);

  // Main Box Packing
  gtk_box_pack_start (GTK_BOX (main_box), mbar, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_box), split_box, TRUE, TRUE, 0);

  // Split Box Packing
  gtk_box_pack_start (GTK_BOX (split_box), left_box, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (split_box), right_box, TRUE, TRUE, 0);

  // Typewriter Box Packing
  //gtk_box_pack_start (GTK_BOX(typewriter_window), typewriter, TRUE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(typewriter_window), typewriter);

  // Grid
  gtk_grid_attach(GTK_GRID(grd), typewriter_window, 0, 0, 1, 1);

  // Left Box Packing
  gtk_box_pack_start (GTK_BOX (left_box), tv, FALSE, FALSE, 0);

  //Right Box Packing
  gtk_box_pack_start (GTK_BOX (right_box), lbl, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (right_box), grd, TRUE, TRUE, 1);
  gtk_box_pack_start (GTK_BOX (right_box), btn, TRUE, TRUE, 0);

  

  // Widget Sizing
  gtk_widget_set_size_request (typewriter_window, 800, 600);

  // Container Adds
  gtk_container_add (GTK_CONTAINER (win), main_box);
  //gtk_container_add (GTK_CONTAINER(typewriter_window), typewriter);
  gtk_widget_show_all(win);
  gtk_main();
}
