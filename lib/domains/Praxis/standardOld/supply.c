#include <std.h>

inherit ROOM;

void init() {
    ::init();
    add_action("go_north", "go");
}

void create() {
    ::create();
    set_properties( ([ "light" : 2, "indoors" : 1, "no castle" : 1 ]) );
    set_short("Horace's General Supply Shop");
    set_long(
      "Welcome to Horace's General Supply Shop!\n"
	"Horace buys and sells goods created for and found "
	"by adventurer's when he is in.  The store opens out "
	"onto Boc La Road south.  There is a passage to the "
	"north guarded by a magic field.");
    set_items(
	(["shop" : "You can buy and sell things here.",
	  "passage" : "Horace keeps the things he has for sale back there.",
	  "road" : "Boc La Road."]) );
    set_exits( 
	      (["south" : "/domains/Praxis/e_boc_la2"]) );
    set_property("no castle", 1);
}

void reset() {
  ::reset();
    if(!present("horace")) 
      new("/domains/Praxis/obj/mon/horace")->move(this_object());
}

int go_north(string str) {
    if(str !="north") return notify_fail("What?\n");
    if(!creatorp(this_player())) {
        message("my_action", "The magic of Horace stops you.", this_player());
        message("other_action", (string)this_player()->query_cap_name()+
          " is stopped by the magic of Horace.", this_object(),
          ({ this_player() }));
	return 1;
    }
    message("other_action", (string)this_player()->query_cap_name()+
      " cannot be stopped by Horace's magic.", this_object(),
      ({ this_player() }));
    this_player()->move_player("/domains/Praxis/storage", "north");
    return 1;
}

