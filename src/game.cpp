#include "game.h"
#include<iostream>

#include <glew.h>
#include <glfw3.h>
#include <sstream>
#include<fstream>
#include <ft2build.h>
#include FT_FREETYPE_H  

// i semi organized the folders, i gave up half way so half of these will turn the program into bloatware
#include "renderer/VertexArray.h"
#include "renderer/IndexBuffer.h"
#include "renderer/Shader.h"
#include "renderer/Renderer.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Texture.h"
#include "core/Controller.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spdlog.h"
#include "core/Camera.h"
#include "core/Player.h"
#include "core/World.h"
#include "renderer/Interface.h"
#include "core/Inventory.h"
#include "core/Projectiles.h"
#include "core/Visuals.h"

static void glfwError(int id, const char* description) // lazy opengl error handling doesnt do shit
{
    printf("[!] opengl error \'%s\'\n", description);
}

void Game::updateStats() {
    static double lastTime = 0.0;

    double currentTime = glfwGetTime();
    m_deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}
void printCraftingRecipes() {
    std::cout << "=== Crafting Recipes ===\n";
    std::cout << "---> WoodSword x1\n"
        << "     Wood x10\n"
        << "---> StoneSword x1\n"
        << "     Wood x5\n"
        << "     Stone x10\n"
        << "---> Rapier x1\n"
        << "     Iron x15\n"
        << "---> DiamondSword x1\n"
        << "     Diamond x5\n"
        << "---> UraniumSword x1\n"
        << "     Uranium x5\n"
        << "---> CrimsonSword x1\n"
        << "     Crimson x5\n"
        << "---> ElectricSword x1\n"
        << "     Diamond x3\n"
        << "     Uranium x3\n"
        << "---> HellHarbinger x1\n"
        << "     CrimsonSword x1\n"
        << "     Solarium x10\n"
        << "---> EndSword x1\n"
        << "     Enderium x10\n"
        << "     Uranium x5\n"
        << "---> CalamitySword x1\n"
        << "     Solarium x20\n"
        << "     Uranium x20\n"
        << "     Enderium x20\n"
        << "     Crimson x20\n"
        << "---> SinScythe x1\n"
        << "     Crimson x15\n"
        << "     Solarium x10\n"
        << "---> Terminator x1\n"
        << "     Enderium x10\n"
        << "     Crimson x15\n"
        << "---> Hyperion x1\n"
        << "     CalamitySword x1\n"
        << "     EndSword x1\n"
        << "     HellHarbinger x1\n"
        << "     CrimsonSword x1\n"
        << "=========================\n";
}

int Game::run() {
    std::string seed;
    printf("---------------------------------------------------\n");
    printf("|                                                 |\n");
    printf("|             CRAFTMINE BUGROCK EDITION           |\n");
    printf("|               By Anh                            |\n");
    printf("---------------------------------------------------\n");
    printf("[NOTE!] BEFORE YOU PLAY THIS GAME, REALISE IT TAKES AROUND 30m TO COMPLETE\n");
    printf("[NOTE!] THERE ARE A LOT OF BUGS\n");
    printf("[NOTE!] FOR EVERY 5 KILLS YOU LEVEL UP, OR BY DEFEATING A BOSS YOU LEVEL UP A CERTAIN AMOUNT OF LEVELS\n");
    printf("[NOTE!] LEVELING SCALES EXPONENTIALLY, DO NOT RUSH INTO THE NEXT DIMENSION UNDERLEVELED\n");
    printf("[NOTE!] FOR EACH DAY, MOBS WILL SPAWN A BIT MORE AND BY NIGHT THEY SPAWN 5x MORE\n");
    std::string admin;

    printf("DO YOU WANT TO GO INTO CREATIVE MODE? (y\\n)\n");
    std::getline(std::cin, admin);


    std::cout << "Choose a seed for you world: ";
    std::getline(std::cin, seed);
    std::cout << '\n';
    int seed_value;
    try {
        seed_value = std::stoi(seed);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        return -1;
    }
    
    // INITIALIZATION START
    if (!glfwInit()) {
        printf("could not iniitialize glfw\n");
        return EXIT_FAILURE;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(&glfwError);
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        spdlog::error("could not initialize freetype library");
        return -1;
    }

    Renderer renderer;

    Vector2 windowDimensions = renderer.getViewPortSize();
    GLFWwindow* window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, "cool ahh game", NULL, NULL);

    if (!window)
    {
        printf("could not create window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("could not initialize glew\n");
        return EXIT_FAILURE;
    }

    spdlog::info("opengl version {}", (const char*)glGetString(GL_VERSION));
    Controller::initialize(window);
    spdlog::debug("controller initialized");

    Shader shader("shaders/voxel.glsl");
    Shader fontshader("shaders/interface.glsl");
    
    Texture* tex = new Texture("res/textures.png", "res/textures.json");
    Renderer::setWorldTextures(tex);
    tex->bind();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QuadRenderer quadrender(&renderer);
    quadrender.loadMeshesFromTexture(tex);
    

    Player player;
    Camera camera = Camera(&player);
    World world(seed_value);

    Inventory::initializeItems();
    Interactable::initializeInteractables();
    Entity::initializeEntitys();
   
    Interface interface = Interface(&fontshader, &quadrender, ft,windowDimensions);
    globalPixelFont = interface.addFont("res/pixelnumba2.ttf");

    quadrender.worldshader = &shader;
    quadrender.worldtexture = tex;

    ProjectileSystem* projectiles = new ProjectileSystem(&world,&quadrender);
    DamageIndicatorSystem::setRender(&quadrender);
    Mesh::initBoxMesh();
    
    // INITIALIZATION END
    
    
    shader.setUniform1f("u_opacity", 1); // never am i going to use this
    shader.setUniform1i("u_texture", 0); // tells opengl to bind to texture unit GL_TEXTURE0
    
    float opacity = 1;
    float inc = 0;

    if (admin == "y") {
        Entity::spawn(EntityType::WITHER, Vector2(400, 400));
        Entity::spawn(EntityType::DRAGON, Vector2(400, 400));
        Entity::spawn(EntityType::UNDEADBOSS, Vector2(400, 400));

        player.getInv().addItem(Inventory::ITEMLIST.WoodSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.EndSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.HellHarbinger, 1);
        player.getInv().addItem(Inventory::ITEMLIST.Rapier, 1);
        player.getInv().addItem(Inventory::ITEMLIST.CalamitySword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.ElectricSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.CrimsonSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.UraniumSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.StoneSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.Hyperion, 1);
        player.getInv().addItem(Inventory::ITEMLIST.DiamondSword, 1);
        player.getInv().addItem(Inventory::ITEMLIST.SinScythe, 1);
        player.getInv().addItem(Inventory::ITEMLIST.Terminator, 1);
        player.getInv().addItem(Inventory::ITEMLIST.NetherWarp, 1);
        player.getInv().addItem(Inventory::ITEMLIST.EnderWarp, 1);

        player.setPlayerLevel(2000);
    }


    while (!glfwWindowShouldClose(window)) {
        renderer.clear();
        glDepthMask(GL_TRUE);
        updateStats();

        Controller::updateController(window,&camera,windowDimensions);
        
        tex->bind();// bind world texture so stuff can render because i poorly organized the code
        camera.updateCamera();
        Entity::updateEntitys(getDeltaTime(),world,&player);
        world.updateWorld(getDeltaTime());
        world.renderWorld(quadrender, shader, &camera);
        Entity::renderEntitys(quadrender, shader, &camera);
        player.updatePlayer(world, quadrender, shader,interface);

        projectiles->update(m_deltaTime);
        projectiles->render();

        DamageIndicatorSystem::update(m_deltaTime);
        DamageIndicatorSystem::render(interface, globalPixelFont);
        // got lazy not abstracting these
        interface.renderText(std::format("Health: {:.2f}",player.getHealth()), globalPixelFont, Vector2(0, windowDimensions.y - 40),1, Vector4::RBGAtoV4(252, 3, 53,255),1);
        interface.renderText(std::format("Level: {}", player.getPlayerLevel()), globalPixelFont, Vector2(0, windowDimensions.y - 80), 1, Vector4::RBGAtoV4(3, 252, 111, 255));
        interface.renderText("Mana: ", globalPixelFont, Vector2(0, windowDimensions.y - 120), 1, Vector4::RBGAtoV4(59, 217, 169, 255));
        interface.renderText(std::format("Day: {} Time: {}:{}",world.getDay(),world.getTimeHours(),world.getTimeMin()), globalPixelFont, Vector2(0, windowDimensions.y - 160), 1, Vector4::RBGAtoV4(255, 255,255, 255));
        interface.renderText(std::format("Chunk: ({},{})", (int)std::floor(player.getPosition().x / CHUNK_SIZE), (int)std::floor(player.getPosition().y / CHUNK_SIZE)), globalPixelFont, Vector2(0, windowDimensions.y - 200), 1, Vector4::RBGAtoV4(255, 255, 255, 255));
        


        interface.update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
