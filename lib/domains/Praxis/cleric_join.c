#include <std.h>

inherit ROOM;

void init() {
    ::init();
    add_action("become", "become");
    add_action("go_north","go");
    add_action("preview","preview");
}

void create() {
    ::create();
set_properties( (["light": 2, "indoors" : 1, "no castle" : 1]) );
        set_short( "the clerics' church of Praxis");
    set_long(
"You are in the small cleric's church of Praxis. It is a very "
"simple, yet elegant building. The lack of adornment is made up "
"for in the strong faith you can feel radiating from this little "
"church. People of all faiths come here for prayer, or solitude. "
"The clerics initiate young adventurers here into the class of "
"clerics. <preview> will tell you about becoming a cleric.  "
"There is a passage north.");
    set_items(
	(["church" : "It holds a small chapel for wedding ceremonies "
	    "west, and there is an inner sanctum for clerics north",
	  "chapel" : "The clerics of the church perform wedding "
	    "ceremonies there.",
	  "light" : "A %^BLUE%^blue%^RESET%^ light protects this place.",
	  "passage" : "It leads north into the cleric's inner sanctum."]) );
    set_exits( 
	      (["east" : "/domains/Praxis/n_centre1"]) );
}

int preview() {
    if((string)this_player()->query_class() != "child") {
        message("info", "This is not for you.", this_player());
	return 1;
    }
    message("info", (string)this_player()->query_cap_name()+" seeks to learn about clerics.", this_object(), ({ this_player() }));
    message("info", "Welcome, child!", this_player());
    message("info", "Clerics are deeply religious people of greatly varied "
      "beliefs, yet united in their belief in the sanctity of the natural world.  Their "
	"ability to get in touch with the unity of nature through prayer "
	"gives them great healing powers as well as powers of protection.  "
	"Destruction is the most horrible of evils to them, and they will "
	"lose the powers they have through prayer if they become too "
	"destructive.  Type <become cleric> to become a cleric.", this_player());
    return 1;
}

int become(string str) {
    if(!str) {
	notify_fail("Become what?\n");
	return 0;
    }
    if(str != "cleric") {
	notify_fail("You cannot become that here.\n");
	return 0;
    }
    if((string)this_player()->query_class() != "child") {
	write("You are much too old to start learning our ways now!");
	return 1;
    }
    write("The High Priestess of Clerics initiates you into the class of clerics.");
    say(this_player()->query_cap_name()+" becomes a cleric.", this_player());
    this_player()->set_class("cleric");
    this_player()->setenv("TITLE", "$N the novice cleric");
    this_player()->init_skills("cleric");
    this_player()->setenv("start", "/domains/Praxis/cleric_hall");
    return 1;
}

int go_north(string str) {
    if(str !="north") return notify_fail("What?\n");
    if((string)this_player()->query_class() != "cleric") {
	write("You cannot penetrate the force field that blocks the passage.");
	say(this_player()->query_cap_name()+" tries to get into the "
	    "church's inner sanctum, but fails.", this_player());
  	return 1;
    }
    this_player()->move_player("/domains/Praxis/cleric_hall", "through the light in the passage");
    return 1;
}
