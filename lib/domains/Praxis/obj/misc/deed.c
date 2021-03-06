/*    /domains/Praxis/order.c
 *    from Nightmare IV
 *    a deed for a high mortal room
 *    created by Descartes of Borg 940701
 */

#include <std.h>
#include <dirs.h>
#include <daemons.h>
#include <security.h>

inherit OBJECT;

int __Light, __Indoors;
string __Short, __Long, __Exit, __NewRoom, __EstateLong;

private string create_file();

void create() {
    Object::create();
    set_name("deed");
    set_id( ({ "estate deed", "deed" }) );
    set_short("an estate deed");
    set_long("A deed to your very own high mortal estate, with a work "
      "order to have workers begin to create your estate "
      "to your desires.  In order to build your estate, go "
      "to the place where you want it built, and type: "
      "\"build estate\".  You will be asked some questions "
      "about the room which will serve as the entrance to your estate."
    );
    set_mass(10);
    set_value(99);
    set_destroy();
}

void init() {
    Object::init();
    add_action("cmd_build", "build");
}

protected int cmd_build(string str) {
    if(str != "estate") return 0;
    if(!high_mortalp(this_player()))
      return notify_fail("Only high mortals may build!\n");
    if((int)environment(this_player())->query_property("indoors")) {
        message("system", "You cannot build an estate indoors!", this_player());
        return 1;
    }
    if(sizeof(filter_array(all_inventory(environment(this_player())),
      "estates", this_object())) >=
      (int)environment(this_player())->query_property("allow estate")) {
        message("system", "This area cannot support an estate.",this_player());
        return 1;
    }
    if(file_size(ESTATES_DIRS+"/"+geteuid(this_player())) != -2) {
        seteuid(UID_ESTATES);
        mkdir(ESTATES_DIRS+"/"+geteuid(this_player()));
        seteuid(getuid());
    }
    __Exit = "$"+file_name(environment(this_player()))+";$exit";
    message("system", "Give a one to two line description of your estate:",
      this_player());
    input_to("input_long");
    return 1;
}

protected void input_long(string str) {
    if(str == "") {
        message("system", "Invalid entry.", this_player());
        return;
    }
    __EstateLong = str;
    message("system", "Please give a short description for the room "
      "people will enter from here.  This is the description like "
      "\"the entrance to "+(string)this_player()->query_CapName()+"'s"
      " estate\" that is seen when in brief mode.", this_player());
    message("prompt", "Enter in a short description: ", this_player());
    input_to("input_short");
}

protected void input_short(string str) {
    __Short = str;
    message("prompt", "Is the room 0) outdoors, or 1) indoors? ", this_player());
    input_to("input_indoors");
    return;
}

protected void input_indoors(string str) {
    int x;

    x = to_int(str);
    if(x && x != 1) {
        message("system", "That was not a valid value.", this_player());
        return;
    }
    __Indoors = x;
    message("system", "How well lit is the room?", this_player());
    message("system", "    0 is darkest, 3 brightest", this_player());
    message("prompt", "\nEnter light value: ", this_player());
    input_to("input_light");
}

protected void input_light(string str) {
    int x;

    x = to_int(str);
    if(x && x != 1 && x != 2 && x != 3) {
        message("system", "Invalid light number.", this_player());
        return;
    }
    __Light = x;
    message("system", "Enter in a long description for the room.  "
      "A long description is what people see when they enter a "
      "room in verbose mode.  A line showing obvious exits is automatically "
      "appended.  In addition, if you add any smells or sounds, the "
      "default ones are automatically appended.  So do not describe sounds or "
      "smells here.", this_player());
    message("system", "Enter in the long description like mail.", this_player());
    rm(DIR_TMP+"/"+geteuid(this_player())+".estate");
    this_player()->edit(DIR_TMP+"/"+geteuid(this_player())+".estate",
      "done_edit", this_object());
}

void abort() {
    rm(DIR_TMP+"/"+geteuid(this_player())+".estate");
    message("system", "Building aborted.", this_player());
}

void done_edit(mixed *unused) {
    string str;

    if(!(str = read_file(DIR_TMP+"/"+geteuid(this_player())+".estate"))) {
        message("system", "No long!", this_player());
        return;
    }
    __Long = replace_string(str, "\n", " ");
    __NewRoom= create_file();
    seteuid(UID_ESTATES);
    write_file(__NewRoom, "set_property: $indoors;#"+__Indoors+"\n");
    write_file(__NewRoom, "set_property: $light;#"+__Light+"\n");
    write_file(__NewRoom, "set_short: $"+__Short+"\n");
    write_file(__NewRoom, "set_long: $"+__Long+"\n");
    write_file(__NewRoom, "add_exit: "+__Exit+"\n");
    ESTATES_D->add_estate((string)this_player()->query_CapName(), __NewRoom,
      file_name(environment(this_player())), __EstateLong);
    seteuid(getuid());
    message("system", "Room begun.  You will see your addition "
      "appear later when the work is complete.", this_player());
    this_object()->remove();
}

private string create_file() {
    string dir, str;
    int x;

    x = sizeof(get_dir((dir = ESTATES_DIRS+"/"+geteuid(this_player())+"/")));
    while(file_exists(str = dir+"room"+x)) x++;
    return str;
}

protected int estates(object ob) {
    return (int)ob->id("estate");
}
