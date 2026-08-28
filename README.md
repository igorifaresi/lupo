# Lupo

A small, header only, C11, SQLite replacement that lets will define your structure as a directory tree.

## Features

- Type safe, you can create the database entities that will be used for the operations.
- Indexes not directories: besides the directory like database structure, each entity that has children store the pointers to then in hash table structure instead of lists, that is what is commonly used in file systems.
- No AI used.

## Usage example

```c
    // starting the database, you have to pass the file system and allocation callbacks
    Lupo_Context db = { 0 };
    db.ram_alloc = test_ram_alloc;
    db.create_empty_page = test_create_empty_page;
    db.load_page = test_load_page;
    db.commit_page = test_commit_page;
    lupo_startup_db(&db);

    // creating a type
    Lupo_Entity user_type = lupo_create_type(
        &db,
        "users",
        LUPO_FIELD_DECL("name", STRING, 0),
        LUPO_FIELD_DECL("age", UINT32, 0)
    );

    // linking an entity to a index
    lupo_link(&db, user_type, "/types/user");

    // creating an entity of the type "user"
    Lupo_Entity user = lupo_create(
        &db,
        lupo_read(&db, "/types/user"),
        LUPO_STR("Igor"),
        LUPO_U32(24)
    );

    // putting the user in the index tree
    lupo_link(&db, user, "/users/igor");

    // updating
    lupo_get_field(&user, "name")->string = "Yay";
    lupo_update(&db, &user);

    // removing from the tree
    lupo_unlink(&db, "/users/igor");

    // removing the entity from the database
    lupo_remove(&db, user);

    lupo_print_link_tree(&db);

```
