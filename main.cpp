#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h>
#include <chrono>
#include <algorithm>

//fix dead func from triggering before game starts

int hp {100};
int enemy_hp {100};
int armour {0};
int attack {0};
int res {0};
bool isEnemyAlive = false;
std::string thing;
std::string choice;
std::string place;
std::string goingTo;
std::string loc;
std::string breakable = "";
std::string items_list[10] = { "sword", "spear", "potion", "book", "apple", "water", "scroll", "chestplate", "helmet", "gem" };
std::string keep_items = "";
std::string forge_items = "";
std::string start_items = "";
std::string courtyard_items = "";
std::string kitchen_items = "";
std::string fountain_items = "";
std::string armoury_items = "";
std::string hallway_items = "";
std::string storeroom_items = "";
std::string library_items = "";
std::string alchemy_items = "";
std::string inv_items = "";
std::string list = "";

void setup() {
    start_items = "";
    forge_items = "&chestplate&&sword&";
    keep_items = "&gem&";
    courtyard_items = "";
    kitchen_items = "&apple&";
    fountain_items = "&water%";
    armoury_items = "&helmet&";
    hallway_items = "";
    storeroom_items = "&scroll&";
    library_items = "&book&";
    alchemy_items = "&potion&";
    inv_items = "&spear&";
    breakable = "&potion&&apple&&water&";
}

void dead() {
    for (;;) {
        if (hp == 0) {
            std::cout << "You died. Game Over" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            exit(1);
        }
    }
}

bool item_exists(std::string items, std::string use) {
    int nub = sizeof(items_list)/sizeof(items_list[0]);
    use = "&" + use + "&";
    std::string item;
    for (int i = 0; i < nub; i++) {
        item = item + "&" + items_list[i] + "&";
    }
    if (item.find(use) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

bool itemInRoom(std::string list, std::string use) {
    if (list.find(use) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

bool isBreakable(std::string list, std::string use) {
    if (list.find(use) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

bool validCommand(std::string command) {
    if (command == "use") {
        return true;
    } else if (command == "pickup") {
        return true;
    } else if (command == "drop") {
        return true;
    } else if (command == "health") {
        return true;
    } else if (command == "go") {
        return true;
    } else if (command == "exit") {
        return true;
    } else if (command == "list") {
        return true;
    } else {
        return false;
    }
}

void eraseSubStr(std::string mainStr, std::string toErase) {
    std::string new_erase;
    new_erase = "&" + toErase + "&";
    // Search for the substring in string
    size_t pos = mainStr.find(new_erase);
    if (pos != std::string::npos) {
        // If found then erase it from string
        if (choice == "pickup") {
            if (place == "start") {
                start_items = mainStr.erase(pos, new_erase.length());
            } else if (place == "forge") {
                forge_items = mainStr.erase(pos, new_erase.length());
            } else if (place == "keep") {
                keep_items = mainStr.erase(pos, new_erase.length());
            } else if (place == "inv") {
                inv_items = mainStr.erase(pos, new_erase.length());
            }
        } else if (choice == "drop") {
            inv_items = mainStr.erase(pos, new_erase.length());
        } else if (choice == "use") {
            inv_items = mainStr.erase(pos, new_erase.length());
        }
    }
}

void addSubStr(std::string room, std::string item) {
    if (room == "inv") {
        inv_items = inv_items + "&" + item + "&";
    } else if (room == "forge") {
        forge_items = forge_items + "&" + item + "&";
    } else if (room == "keep") {
        keep_items = keep_items + "&" + item + "&";
    } else if (room == "start") {
        start_items = start_items + "&" + item + "&";
    }
}

void itemActions(std::string action, std::string item) {
    if (action == "pickup") {
        if (place == "forge") {
            if (itemInRoom(forge_items, item) == true) {
                //add the item to inventory
                addSubStr("inv", item);
                eraseSubStr(forge_items, item);
                res = 1;
            } else {
                std::cout << "There is no " << item << " in this room." << std::endl;
            }
        } else if (place == "keep") {
            if (itemInRoom(keep_items, item) == true) {
                //add the item to inventory
                addSubStr("inv", item);
                eraseSubStr(keep_items, item);
                res = 1;
            } else {
                std::cout << "There is no " << item << " in this room." << std::endl;
            }
        } else if (place == "start") {
            if (itemInRoom(start_items, item) == true) {
                //add the item to inventory
                addSubStr("inv", item);
                eraseSubStr(start_items, item);
                res = 1;
            } else {
                std::cout << "There is no " << item << " in this room." << std::endl;
            }
        }
    } else if (action == "drop") {
        if (itemInRoom(inv_items, item) == true) {
            //remove item from inventory
            eraseSubStr(inv_items, item);
            //add the item to the room
            if (place == "forge") {
                addSubStr("forge", item);
                res = 1;
            } else if (place == "keep") {
                addSubStr("keep", item);
                res = 1;
            } else if (place == "start") {
                addSubStr("start", item);
                res = 1;
            }
        } else {
            std::cout << "You do not have a " << item << " in you inventory." << std::endl;
            res = 2;
        }
    } else if (action == "use") {
        if (itemInRoom(inv_items, item) == true) {
            if (isBreakable(breakable, item) == true) {
                // destroy item if it is single use
                if (item == "potion") {
                    hp = hp + 20;
                    std::cout << "Health is now " << hp << std::endl;
                    eraseSubStr(inv_items, item);
                } else if (item == "apple") {
                    hp = hp + 15;
                    std::cout << "Health is now " << hp << std::endl;
                    eraseSubStr(inv_items, item);
                } else if (item == "water") {
                    hp = hp + 10;
                    std::cout << "Health is now " << hp << std::endl;
                    eraseSubStr(inv_items, item);
                } else if (item == "book") {
                    hp = hp + 15;
                    std::cout << "You learn better ways to defend yourself." << std::endl;
                    std::cout << "Health is now " << hp << std::endl;
                    eraseSubStr(inv_items, item);
                } else if (item == "scroll") {
                    hp = hp + 15;
                    std::cout << "You learn better ways to attack." << std::endl;
                    std::cout << "Health is now " << hp << std::endl;
                    eraseSubStr(inv_items, item);
                }
            } else {
                // keep item if it can be used again
                if (item == "sword") {
                    if (isEnemyAlive == true) {
                        attack = rand() % 30 + 1;
                        enemy_hp = enemy_hp - attack;
                        if (enemy_hp < 1) {
                            enemy_hp = 0;
                        }
                        std::cout << "Enemy is now " << enemy_hp << " health" << std::endl;
                        if (armour == 0) {
                            attack = rand() % 10 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        } else if (armour == 20) {
                            attack = rand() % 5 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        } else if (armour == 10) {
                            attack = rand() % 7 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        } else if (armour == 30) {
                            attack = rand() % 3 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        }
                        if (enemy_hp < 1) {
                            isEnemyAlive = false;
                            enemy_hp = 100;
                        }
                    } else {
                        std::cout << "There is no enemy to attack." << std::endl;
                    }
                } else if (item == "spear") {
                    if (isEnemyAlive == true) {
                        attack = rand() % 15 + 1;
                        enemy_hp = enemy_hp - attack;
                        if (enemy_hp < 1) {
                            enemy_hp = 0;
                        }
                        std::cout << "Enemy is now " << enemy_hp << " health" << std::endl;
                        if (armour == 0) {
                            attack = rand() % 10 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        } else if (armour == 20) {
                            attack = rand() % 5 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        } else if (armour == 10) {
                            attack = rand() % 7 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        } else if (armour == 30) {
                            attack = rand() % 3 + 1;
                            hp = hp - attack;
                            std::cout << "You have been hit. Your hp is " << hp << std::endl;
                        }
                        if (enemy_hp < 1) {
                            isEnemyAlive = false;
                            enemy_hp = 100;
                        }
                    } else {
                        std::cout << "There is no enemy to attack." << std::endl;
                    }
                } else if (item == "chestplate") {
                    armour = armour + 20;
                    std::cout << "You have equiped the chestplate.";
                } else if (item == "helmet") {
                    armour = armour + 10;
                    std::cout << "You have equiped the helmet.";
                } else if (item == "gem") {
                    std::cout << "You become invincible. The End." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                    exit(1);
                }
            }
            res = 1;
        } else {
            std::cout << "You do not have a " << item << " in you inventory." << std::endl;
            res = 2;
        }
    }
}

void startGame(); //done
void keep(); //done
void kitchen(); //done
void courtyard(); //done
void hallway(); //done
void fountain(); //done
void armoury(); //done
void forge(); //done
void storeroom(); //done
void library(); //done
void alchemy(); //done

void alchemy() {
    place = "alchemy";
    std::cout << "You are now in the alchemy." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "library") {
                    library();
                } else if (thing == "storeroom") {
                    storeroom();
                } else if (thing == "keep") {
                    keep();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the library, storeroom, or keep." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void library() {
    place = "library";
    std::cout << "You are now in the library." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "keep") {
                    keep();
                } else if (thing == "alchemy") {
                    alchemy();
                } else if (thing == "storeroom") {
                    storeroom();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the keep, alchemy, or storeroom." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void storeroom() {
    place = "storeroom";
    std::cout << "You are now in the storeroom." << std::endl;
    std::cout << "There is an enemy in front of you" << std::endl;
    isEnemyAlive = true;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "keep") {
                    keep();
                } else if (thing == "library") {
                    library();
                } else if (thing == "alchemy") {
                    alchemy();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the keep, library, or alchemy." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void forge() {
    place = "forge";
    std::cout << "You are now in the forge." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "hallway") {
                    hallway();
                } else if (thing == "armoury") {
                    armoury();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the hallway or armoury." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void armoury() {
    place = "armoury";
    std::cout << "You are now in the armoury." << std::endl;
    std::cout << "There is an enemy in front of you" << std::endl;
    isEnemyAlive = true;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "fountain") {
                    fountain();
                } else if (thing == "forge") {
                    forge();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the fountain or forge." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void fountain() {
    place = "fountain";
    std::cout << "You are now next to the fountain." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "courtyard") {
                    courtyard();
                } else if (thing == "armoury") {
                    armoury();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the courtyard or armoury." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void courtyard() {
    place = "courtyard";
    std::cout << "You are now in the courtyard." << std::endl;
    std::cout << "There is an enemy in front of you" << std::endl;
    isEnemyAlive = true;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "kitchen") {
                    kitchen();
                } else if (thing == "start") {
                    startGame();
                } else if (thing == "fountain") {
                    fountain();
                } else if (thing == "armoury") {
                    armoury();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the kitchen, start, fountain, or armoury." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void kitchen() {
    place = "kitchen";
    std::cout << "You are now in the kitchen." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "keep") {
                    keep();
                } else if (thing == "hallway") {
                    hallway();
                } else if (thing == "start") {
                    startGame();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the keep, hallway, or start." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void keep() {
    place = "keep";
    std::cout << "You are now in the keep." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "library") {
                    library();
                } else if (thing == "storeroom") {
                    storeroom();
                } else if (thing == "alchemy") {
                    alchemy();
                } else if (thing == "hallway") {
                    hallway();
                } else if (thing == "start") {
                    startGame();
                } else if (thing == "kitchen") {
                    kitchen();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the library, storeroom, alchemy, hallway, start, or kitchen." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

void hallway() {
    place = "hallway";
    std::cout << "You are now in the hallway." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in you inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?";
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "keep") {
                    keep();
                } else if (thing == "forge") {
                    forge();
                } else if (thing == "start") {
                    startGame();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the keep, forge, or start." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
}

//then finish map and setup the story line

void startGame() {
    place = "start";
    std::cout << "You are standing in the entrance of a large castle." << std::endl;
    for (;;) {
        std::cout << "What do you want to do?" << std::endl;
        std::cin >> choice;
        if (validCommand(choice) == true) {
            if (choice == "use") {
                std::cout << "What would you like to use?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "drop") {
                std::cout << "What would you like to drop?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in your inventory" << std::endl;
                }
            } else if (choice == "pickup") {
                std::cout << "What would you like to pickup?";
                std::cin >> thing;
                if (item_exists(items_list[5], thing)) {
                    itemActions(choice, thing);
                } else {
                    std::cout << "that item is not in this room" << std::endl;
                }
            } else if (choice == "health") {
                std::cout << "You have " << hp << " hp" << std::endl;
            } else if (choice == "exit") {
                exit(0);
            } else if (choice == "list") {
                std::cout << "What would you like to list?" << std::endl;
                std::cin >> thing;
                if (thing == "inventory") {
                    list = inv_items;
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                } else if (thing == "room") {
                    if (place == "forge") {
                        list = forge_items;
                    } else if (place == "keep") {
                        list = keep_items;
                    } else if (place == "start") {
                        list = start_items;
                    }
                    std::replace(list.begin(), list.end(), '&', ' ');
                    std::cout << list << std::endl;
                }
                else {
                    std::cout << "I only know list inventory and list room." << std::endl;
                }
            } else if (choice == "go") {
                std::cout << "Where would you like to go?";
                std::cin >> thing;
                if (thing == "keep") {
                    keep();
                } else if (thing == "kitchen") {
                    kitchen();
                } else if (thing == "hallway") {
                    hallway();
                }  else if (thing == "courtyard") {
                    courtyard();
                } else {
                    std::cout << "You cannot go there. " << "You can go to the hallway, courtyard, kitchen, or keep." << std::endl;
                }
            }
        } else {
            std::cout << "I only know pickup, drop, use, go, health as commands." << std::endl;
        }
    }
    
}

int main() {

    setup();
    std::thread x(startGame);
    std::thread y(dead);

    // Both function are now running an different thread
    // We need to wait for them to finish

    x.join();
    y.join();

    return 0;
}