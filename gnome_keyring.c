#include "ruby.h"
#include <gnome-keyring.h> //libgnome-keyring-dev

static const char RubyKR[] = "RubyKeyring";
static const char RubyID[] = "ID";
static GnomeKeyringPasswordSchema schema = {
  GNOME_KEYRING_ITEM_GENERIC_SECRET,
  {
    { RubyKR, GNOME_KEYRING_ATTRIBUTE_TYPE_STRING },
    { RubyID, GNOME_KEYRING_ATTRIBUTE_TYPE_STRING },
    { NULL, 0 },
  }
};

static gchar display[] = "used by RubyKeyring.";

static VALUE keyring_get(VALUE self,VALUE key)
{
  VALUE retval;
  GnomeKeyringResult keyres;
  gchar *password = NULL;
  keyres = gnome_keyring_find_password_sync(&schema,
                                            &password,
					    RubyKR,RubyKR,
					    RubyID,StringValueCStr(key),
					    NULL
                                            );

  if( keyres == GNOME_KEYRING_RESULT_OK ){
    retval = rb_str_new_cstr(password);
    gnome_keyring_free_password(password);
  }
  else {
    retval = Qfalse;
  }

  return retval;
}

static VALUE keyring_set(VALUE self,VALUE id,VALUE password)
{
  if( password == Qnil ){
    gnome_keyring_delete_password_sync(&schema,
                                       RubyKR,RubyKR,
				       RubyID,StringValueCStr(id),
				       NULL);
  }
  else{
    gnome_keyring_store_password_sync(&schema,
                                      RubyKR,
                                      display,
                                      StringValueCStr(password),
                                      RubyKR,RubyKR,
                                      RubyID,StringValueCStr(id),
                                      NULL);
  }

  return 0;
}

void Init_gnome_keyring()
{
  VALUE module;

  g_set_application_name("RubyGnomeKeyring");

  module = rb_define_module("GNOME_KEYRING");
  rb_define_module_function(module, "[]",keyring_get,1);
  rb_define_module_function(module ,"[]=",keyring_set,2);
}
