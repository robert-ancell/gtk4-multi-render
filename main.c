#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <gtk/gtk.h>

static const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

static const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor; \n"
    "void main() \n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
    "}\n\0";

static float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                           0.0f,  0.0f,  0.5f, 0.0f};

static GLuint program = 0;
static GLuint vertex_shader = 0;
static GLuint fragment_shader = 0;
static GLuint vao = 0;
static GLuint vbo = 0;

static void realize_cb(GtkGLArea *area) {
  gtk_gl_area_make_current(area);

  GError *error = gtk_gl_area_get_error(area);
  if (error != NULL) {
    g_printerr("Failed to setup GL: %s\n", error->message);
    return;
  }

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// static GdkGLContext * create_context_cb(GtkGLArea *area) {}

static gboolean render_cb(GtkGLArea *area, GdkGLContext *context) {
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  return TRUE;
}

static GtkWidget *add_window(GtkApplication *app, const char *title) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), title);
  gtk_widget_set_visible(window, TRUE);

  GtkWidget *gl_area = gtk_gl_area_new();
  g_signal_connect(gl_area, "realize", G_CALLBACK(realize_cb), NULL);
  // g_signal_connect(gl_area, "create-context", G_CALLBACK(create_context_cb),
  // NULL);
  g_signal_connect(gl_area, "render", G_CALLBACK(render_cb), NULL);
  gtk_window_set_child(GTK_WINDOW(window), gl_area);

  return window;
}

static void activate_cb(GtkApplication *app, gpointer user_data) {
  add_window(app, "Window 1");
  // add_window(app, "Window 2");
  // add_window(app, "Window 3");
}

int main(int argc, char **argv) {
  g_autoptr(GtkApplication) app = gtk_application_new(
      "com.canonical.gtk4_multi_render", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
