# include "../../include/gui/ParchisGUI.h"

#define animation_time 500

const map<Box, vector<Vector2i>> ParchisGUI::box2position = ParchisGUI::generatePositions();

const string ParchisGUI::background_theme_file = "data/music/background_theme";

const string ParchisGUI::background_theme_hurryup_file = "data/music/background_theme_hurryup";

const string ParchisGUI::background_theme_win_file = "data/music/epic_win";

const string ParchisGUI::background_theme_lose_file = "data/music/epic_lose";

const string ParchisGUI::background_theme_star_file = "data/music/star";

const string ParchisGUI::background_theme_mega_file = "data/music/mega";

const string ParchisGUI::background_theme_shock_file = "data/music/background_theme_shock";

const string ParchisGUI::icon_file = "data/textures/icon_parchis.png";

const IntRect ParchisGUI::turns_arrow_rect = IntRect(0, 280, 112, 112);

const map<color, int> ParchisGUI::color2turns_arrow_pos = {
   //{yellow, 50},
   //{blue, 130},
   //{red, 210},
   //{green, 290}
   {yellow, 50},
   {blue, 210},
   {red, 210},
   {green, 50}
};

const string ParchisGUI::sound_move_file = "data/music/teleport";

const string ParchisGUI::sound_boing_file = "data/music/boing";

const string ParchisGUI::sound_forbidden_file = "data/music/nope";

const string ParchisGUI::sound_eaten_file = "data/music/bad_news";

const string ParchisGUI::sound_applause_file = "data/music/applause";

const string ParchisGUI::sound_explosion_file = "data/music/explosion";

const string ParchisGUI::sound_starhit_file = "data/music/starhit";

const string ParchisGUI::sound_shock_file = "data/music/shock";

const string ParchisGUI::sound_boo_file = "data/music/boo";

const string ParchisGUI::sound_horn_file = "data/music/horn";

const string ParchisGUI::sound_bullet_file = "data/music/bullet";

const float ParchisGUI::ASPECT_RATIO = 2.0f;

int ParchisGUI::getInitialWindowWidth(){
   return ParchisGUI::getInitialWindowHeight() * ASPECT_RATIO;
}

int ParchisGUI::getInitialWindowHeight(){
   int result = 0;
   int screen_height = VideoMode::getDesktopMode().height;
   int screen_width = VideoMode::getDesktopMode().width;
   cout << "Screen dimension: " << screen_width << "x" << screen_height << endl;
   // Check if the quotient is larger than 2*16:9
   if ((float)screen_width / screen_height > 16.0f / 9.0f){
      screen_width /= 2; // Quizás dos monitores
   }
   // Compare with the standard monitor resolutions.
   if (screen_width <= 1366 && screen_height <= 768){
      result = 600;
   }
   else if (screen_width <= 1600 && screen_height <= 900){
      result = 750;
   }
   else if (screen_width <= 1920 && screen_height <= 1080){
      result = 800;
   }
   else if (screen_width <= 2304 && screen_height <= 1440){
      result = 1000;
   }
   else if (screen_width <= 2560 && screen_height <= 1440){
      result = 1100;
   }
   else if (screen_width <= 2560 && screen_height <= 1600){
      result = 1200;
   }
   else if (screen_width <= 2880 && screen_height <= 1800){
      result = 1300;
   }
   else if (screen_width <= 3000 && screen_height <= 2000){
      result = 1400;
   }
   else if (screen_width <= 3200 && screen_height <= 1800){
      result = 1400;
   }
   else if (screen_width <= 3840 && screen_height <= 2160){
      result = 1600;
   }
   else{
      cout << "MADRE MÍA, QUÉ PEDAZO DE PANTALLA ESTAS USANDO?" << endl;
      cout << min(screen_height, (int)(screen_width / ASPECT_RATIO)) - 200 << endl;
      result = min(screen_height, (int)(screen_width / ASPECT_RATIO)) - 700;
   }

   // devuelve el valor de result
   return result;
}

ParchisGUI::ParchisGUI(Parchis& model): RenderWindow(VideoMode(getInitialWindowWidth(), getInitialWindowHeight(),
                                                               VideoMode::getDesktopMode().bitsPerPixel),
                                                     L"Parchís", Style::Titlebar | Style::Close | Style::Resize),
                                        game_thread(&ParchisGUI::gameLoop, this){
   // L"string" parece que permite representar caraceteres unicode. Útil para acentos y demás.
   this->model = &model;

   this->clicked = false;

   this->last_dice = -1;
   this->gui_turn = 1;

   //Cargamos las texturas
   this->tBackground.loadFromFile("data/textures/background.png");
   this->tPieces.loadFromFile("data/textures/fichas_parchis_extended.png");
   this->tPieces.setSmooth(true);
   this->tSpecialItems.loadFromFile("data/textures/itemboxes.png");
   this->tSpecialItems.setSmooth(true);
   this->tBoardTraps.loadFromFile("data/textures/itemboxes.png");
   //TODO: CAMBIAR EL PLATANO A ALGO DIFERENTE (Y EL NOMBRE DE LA TEXTURA)
   this->tBoardTraps.setSmooth(true);
   this->tBoard.loadFromFile("data/textures/parchis_board_resized_zebra.png");
   this->tBoard.setSmooth(true);
   this->tDices.loadFromFile("data/textures/dice_extended.png");
   this->tDices.setSmooth(true);
   this->tSkipBt.loadFromFile("data/textures/skip_buttons.png");
   this->tSkipBt.setSmooth(true);
   this->tButtons.loadFromFile("data/textures/buttons.png");
   this->tButtons.setSmooth(true);
   this->tBOOM.loadFromFile("data/textures/JustACircle.png");
   this->tBOOM.setSmooth(true);

   if (!this->window_fonts.loadFromFile("data/fonts/arial.ttf")){
      std::cout << "Error loading font" << std::endl;
   }

   // Definimos los sprites
   this->background = Sprite(tBackground);
   this->background.setPosition(1000, 1000);
   //this->boards.push_back(BoardSprite(tBoard));
   this->board = BoardSprite(tBoard);

   // Vector de colores (ver cómo se podría obtener directamente del enumerado)
   vector<color> colors = {yellow, blue, red, green};

   // Creación de las fichas
   for (auto col : colors){
      vector<PieceSprite> col_pieces_sprites;
      for (int j = 0; j < model.getBoard().getPieces(col).size(); j++){
         col_pieces_sprites.push_back(PieceSprite(tPieces, j, model.getBoard().getPiece(col, j)));
         setPieceAttributesOnBoard(col_pieces_sprites[j], j, 0);
         Vector2f pos = getPiecePositionOnBoard(col_pieces_sprites[j], j, 0);
         col_pieces_sprites[j].setPosition(pos.x, pos.y);
         //col_pieces_sprites[j].setPosition(box3position(col, j, 0));

         /*
         Vector2f middle_point;
         middle_point = (box3position(col, j, 0) + box3position(col, j % 68, 0)) / 2.f;
         //col_pieces_sprites[j].setPosition(middle_point);
         col_pieces_sprites[j].setOrigin(15, 15);
         col_pieces_sprites[j].setScale(2.0, 2.0);
         */

         /*
         col_pieces_sprites[j].setPosition(box3position(col, j, 0) + Vector2f(15, 15));
         col_pieces_sprites[j].setOrigin(15, 15);
         col_pieces_sprites[j].setScale(0.5, 0.5);
         */
      }
      pieces.insert({col, col_pieces_sprites});
   }


   //Creación de los dados
   Vector2i ini_pos(900, 50);
   Vector2i offset(70, 80);

   vector<color> dice_colors = {yellow, blue};
   for (int i = 0; i < dice_colors.size(); i++){
      for (int j = 1; j <= 6; j++){
         dices[dice_colors[i]].push_back(DiceSprite(tDices, j, dice_colors[i]));
         Vector2i pos = ini_pos + Vector2i((j - 1) * offset.x, 2 * i * offset.y);
         dices[dice_colors[i]][j - 1].setPosition(pos.x, pos.y);
      }

      dices[dice_colors[i]].push_back(DiceSprite(tDices, yinyang, dice_colors[i]));
      dices[dice_colors[i]][6].setPosition(ini_pos.x + offset.x * 6, ini_pos.y + offset.y * 2 * i);
      //special_10_20_dice[colors[i]].setNumber(20);
      //special_10_20_dice[colors[i]].setModelColor(colors[i]);
      special_10_20_dice[dice_colors[i]].push_back(DiceSprite(tDices, -1, dice_colors[i]));
      special_10_20_dice[dice_colors[i]][0].setPosition(ini_pos.x + offset.x * 7, ini_pos.y + offset.y * 2 * i);
   }

   // Creación de los botones
   this->skip_turn_button = SkipTurnButton(tSkipBt);
   this->skip_turn_button.setPosition(Vector2f(850, 400));
   this->skip_turn_button.setScale(Vector2f(0.55, 0.55));

   this->move_heuristic_button = MoveHeuristicButton(tButtons);
   this->move_heuristic_button.setPosition(Vector2f(1050, 400));

   this->auto_heuristic_button = AutoHeuristicButton(tButtons);
   this->auto_heuristic_button.setPosition(Vector2f(1050, 464));

   this->music_on_off_button = MusicOnOffButton(tButtons);
   this->music_on_off_button.setPosition(Vector2f(850, 550));

   this->sound_on_off_button = SoundOnOffButton(tButtons);
   this->sound_on_off_button.setPosition(Vector2f(960, 550));

   // Flecha de turnos.
   this->turns_arrow = Sprite(tButtons);
   this->turns_arrow.setTextureRect(turns_arrow_rect);
   this->turns_arrow.setPosition(Vector2f(850, 50));
   this->turns_arrow.setScale(Vector2f(0.5, 0.5));
   this->turns_arrow.setColor(Color::Yellow);

   // Sprites de explosión.
   for (int i = 0; i < BOOM_SPRITE_LIMIT; i++){
      this->blue_boom[i] = ExplosionSprite(tBOOM, Color::Cyan);
      this->red_boom[i] = ExplosionSprite(tBOOM, Color::Red);
      this->golden_boom[i] = ExplosionSprite(tBOOM, Color::White); //Color(255, 215, 0));
      this->horn_boom[i] = ExplosionSprite(tBOOM, Color(255, 140, 0));
   }
   this->current_boom_sprite = 0;


   // Agrupación de los canales de animación.
   this->all_animators.push_back(&this->animations_ch1);
   this->all_animators.push_back(&this->animations_ch2);
   this->all_animators.push_back(&this->animations_ch3);
   this->all_animators.push_back(&this->animations_ch4);
   this->all_animators.push_back(&this->animations_ch5);

   //Creación de las vistas
   general_view = View(FloatRect(1000, 1000, 1600, 800));
   general_view.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));

   board_view = View(FloatRect(0.f, 0.f, 800.f, 800.f));
   board_view.setViewport(FloatRect(0.f, 0.f, 0.5f, 1.f));

   dice_view = View(FloatRect(800.f, 50.f, 720.f, 320.f));
   dice_view.setViewport(FloatRect(800.f / 1600.f, 50.f / 800.f, 720.f / 1600.f, 320.f / 800.f));


   bt_panel_view = View(FloatRect(850.f, 400.f, 600.f, 600.f));
   bt_panel_view.setViewport(FloatRect(850.f / 1600.f, 400.f / 800.f, 600.f / 1600.f, 600.f / 800.f));

   rotate_board = false;
   rotate_angle0 = 0.0;


   collectSprites();

   this->animation_lock = false;
   this->not_playable_lock = false;

   //Música
   this->initializeBackgroundMusic();
   this->setBackgroundMusic(false);
   //Sonidos
   this->initializeSoundEffects();
   this->setSoundEffects(false);

   this->updateSprites();

   //Icono de la ventana.
   if (icon.loadFromFile(icon_file)){
      this->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
   }
   else{
      cout << "Icon could not be loaded" << endl;
   }

   this->avg_fps = 0.0;
   this->total_frames = 0;
   this->setFramerateLimit(60);

   // Inicialización de la hebra.
   // this->call_thread_start = false;

   // Inicialización de shaders.
   // Estrella
   if (!this->star_shader.loadFromFile("data/shaders/star_shader.frag", Shader::Fragment)){
      cout << "Error loading star shader." << endl;
   }

   // Boo
   if (!this->boo_shader.loadFromFile("data/shaders/boo_shader.frag", Shader::Fragment)){
      cout << "Error loading boo shader." << endl;
   }

   // Explosión de estrella
   if (!this->star_boom_shader.loadFromFile("data/shaders/star_boom_shader.frag", Shader::Fragment)){
      cout << "Error loading star boom shader." << endl;
   }

   // Bananed
   if (!this->bananed_shader.loadFromFile("data/shaders/bananed_shader.frag", Shader::Fragment)){
      cout << "Error loading bananed shader." << endl;
   }

   // Dice
   if (!this->dice_shader.loadFromFile("data/shaders/dice_shader.frag", Shader::Fragment)){
      cout << "Error loading dice shader." << endl;
   }

   this->startGameLoop();
}

void ParchisGUI::collectSprites(){
   // Tablero como sprite dibujable (IMPORTANTE: Añadir a all_drawable_sprites en el orden en que se dibujan)
   all_drawable_sprites.push_back(&background);
   general_drawable_sprites.push_back(&background);

   all_drawable_sprites.push_back(&board);
   board_drawable_sprites.push_back(&board);
   all_clickable_sprites.push_back(&board);
   board_clickable_sprites.push_back(&board);

   // Vector de colores (ver cómo se podría obtener directamente del enumerado)
   vector<color> colors = {red, blue, green, yellow};
   vector<color> dice_colors = {yellow, blue};

   // Explosiones como dibujables y no clickables. Por debajo de las fichas.
   golden_boom_sprite_start = board_drawable_sprites.size();
   for (int i = 0; i < BOOM_SPRITE_LIMIT; i++){
      all_drawable_sprites.push_back(&golden_boom[i]);
      board_drawable_sprites.push_back(&golden_boom[i]);
   }
   golden_boom_sprite_end = board_drawable_sprites.size();

   for (int i = 0; i < BOOM_SPRITE_LIMIT; i++){
      all_drawable_sprites.push_back(&blue_boom[i]);
      all_drawable_sprites.push_back(&red_boom[i]);
      all_drawable_sprites.push_back(&horn_boom[i]);
      board_drawable_sprites.push_back(&blue_boom[i]);
      board_drawable_sprites.push_back(&red_boom[i]);
      board_drawable_sprites.push_back(&horn_boom[i]);
   }


   piece_sprite_start = board_drawable_sprites.size();
   for (int i = 0; i < colors.size(); i++){
      color col = colors[i];
      // Añadir fichas como dibujables y clickables.
      for (int j = 0; j < pieces[col].size(); j++){
         all_drawable_sprites.push_back(&pieces[col][j]);
         all_clickable_sprites.push_back(&pieces[col][j]);
         board_drawable_sprites.push_back(&pieces[col][j]);
         board_clickable_sprites.push_back(&pieces[col][j]);
      }
   }
   piece_sprite_end = board_drawable_sprites.size();

   dice_sprite_start = dice_drawable_sprites.size();
   for (int i = 0; i < dice_colors.size(); i++){
      color col = dice_colors[i];
      // Añadir dados como dibujables y clickables.
      for (int j = 0; j < dices[col].size(); j++){
         all_drawable_sprites.push_back(&dices[col][j]);
         all_clickable_sprites.push_back(&dices[col][j]);
         dice_drawable_sprites.push_back(&dices[col][j]);
         dice_clickable_sprites.push_back(&dices[col][j]);
      }

      // Añadir dados especiales como dibujables y clickables.
      all_drawable_sprites.push_back(&special_10_20_dice[col][0]);
      all_clickable_sprites.push_back(&special_10_20_dice[col][0]);
      dice_drawable_sprites.push_back(&special_10_20_dice[col][0]);
      dice_clickable_sprites.push_back(&special_10_20_dice[col][0]);
   }
   dice_sprite_end = dice_drawable_sprites.size();

   // Añadir flecha de turnos como dibujable.
   all_drawable_sprites.push_back(&turns_arrow);
   dice_drawable_sprites.push_back(&turns_arrow);
   turns_arrow_sprite_pos = dice_drawable_sprites.size() - 1;


   // Añadir botones como dibujables y clickables.
   vector<ClickableSprite*> buttons = {
      &skip_turn_button, &move_heuristic_button, &auto_heuristic_button, &music_on_off_button, &sound_on_off_button
   };

   for (int i = 0; i < buttons.size(); i++){
      all_drawable_sprites.push_back(buttons[i]);
      all_clickable_sprites.push_back(buttons[i]);
      bt_panel_drawable_sprites.push_back(buttons[i]);
      bt_panel_clickable_sprites.push_back(buttons[i]);
   }
}

void ParchisGUI::dynamicallyCollectSprites(){
   if (model->updateBoard()){
      special_items.clear();
      // Creación de los special items
      for (int j = 0; j < this->model->getBoard().getSpecialItems().size(); j++){
         special_items.
            push_back(SpecialItemSprite(tSpecialItems, this->model->getBoard().getSpecialItems()[j].type));
         special_items[j].setPosition(box2position.at(this->model->getBoard().getSpecialItems()[j].box).at(0).x,
                                      box2position.at(this->model->getBoard().getSpecialItems()[j].box).at(0).y);
      }

      board_traps.clear();

      for (int j = 0; j < this->model->getBoard().getTraps().size(); j++){
         board_traps.push_back(BoardTrapSprite(tBoardTraps, this->model->getBoard().getTraps()[j].getType()));
         board_traps[j].setPosition(box2position.at(this->model->getBoard().getTraps()[j].getBox()).at(0).x,
                                    box2position.at(this->model->getBoard().getTraps()[j].getBox()).at(0).y);
         board_traps[j].setColor(Color::Red);
      }

      model->sendUpdatedBoardSignal();
   }

   vector<color> dice_colors = {yellow, blue};
   // Creación de los dados
   Vector2i ini_pos(900, 50);
   Vector2i offset(70, 80);


   all_dynamic_drawable_sprites.clear();
   board_dynamic_drawable_sprites.clear();
   dice_dynamic_drawable_sprites.clear();

   all_dynamic_clickable_sprites.clear();
   dice_dynamic_clickable_sprites.clear();

   for (int i = 0; i < special_items.size(); i++){
      all_dynamic_drawable_sprites.push_back(&special_items[i]);
      board_dynamic_drawable_sprites.push_back(&special_items[i]);
   }

   for (int i = 0; i < board_traps.size(); i++){
      all_dynamic_drawable_sprites.push_back(&board_traps[i]);
      board_dynamic_drawable_sprites.push_back(&board_traps[i]);
   }

   dynamic_dice_sprite_start = dice_dynamic_drawable_sprites.size();
   for (int i = 0; i < dice_colors.size(); i++){
      color col = dice_colors[i];
      for (int j = 0; j < special_dices[col].size(); j++){
         all_dynamic_drawable_sprites.push_back(&special_dices[col][j]);
         all_dynamic_clickable_sprites.push_back(&special_dices[col][j]);
         dice_dynamic_drawable_sprites.push_back(&special_dices[col][j]);
         dice_dynamic_clickable_sprites.push_back(&special_dices[col][j]);
      }
   }
   dynamic_dice_sprite_end = dice_dynamic_drawable_sprites.size();
}

void ParchisGUI::mainLoop(){
   processSettings();
   processMouse();
   processEvents();
   processAnimations();
   // TODO: Esto no pyuede estar refrescándose siempre, si no no se puede clicar ni hacer nada.
   // Se tendría que llamar solo cuando de verdad haya habido un cambio en estos elementos.
   dynamicallyCollectSprites();
   paint();

   current_time = game_clock.restart().asSeconds();
   fps = 1.f / (current_time);
   avg_fps = (avg_fps * total_frames + fps) / (total_frames + 1);
   total_frames++;
}

void ParchisGUI::gameLoop(){
   model->gameLoop();
   int winner = model->getWinner();
   if (winner != -1){
      if (model->getPlayers()[winner]->isRemote()){
         switchBackgroundMusic(background_theme_lose);
      }
      else{
         switchBackgroundMusic(background_theme_win);
      }
   }
}

void ParchisGUI::startGameLoop(){
   this->game_thread.launch();
}

void ParchisGUI::processMouse(){
   Vector2i pos = Mouse::getPosition(*this);
   Vector2f world_pos;

   this->setView(board_view);
   world_pos = this->mapPixelToCoords(pos);

   bool already_hovered = false;

   for (int i = board_clickable_sprites.size() - 1; i >= 0; i--){
      ClickableSprite* current_sprite = board_clickable_sprites[i];
      if (current_sprite->getGlobalBounds().contains(world_pos) && !already_hovered){
         current_sprite->setHovered(true, *this);
         already_hovered = true;
      }
      else{
         current_sprite->setHovered(false, *this);
      }
   }

   this->setView(dice_view);
   world_pos = this->mapPixelToCoords(pos);

   for (int i = dice_clickable_sprites.size() - 1; i >= 0; i--){
      ClickableSprite* current_sprite = dice_clickable_sprites[i];
      if (current_sprite->getGlobalBounds().contains(world_pos) && !already_hovered){
         current_sprite->setHovered(true, *this);
         already_hovered = true;
      }
      else{
         current_sprite->setHovered(false, *this);
      }
   }

   for (int i = dice_dynamic_clickable_sprites.size() - 1; i >= 0; i--){
      ClickableSprite* current_sprite = dice_dynamic_clickable_sprites[i];
      if (current_sprite->getGlobalBounds().contains(world_pos) && !already_hovered){
         current_sprite->setHovered(true, *this);
         already_hovered = true;
      }
      else{
         current_sprite->setHovered(false, *this);
      }
   }

   this->setView(bt_panel_view);
   world_pos = this->mapPixelToCoords(pos);

   for (int i = bt_panel_clickable_sprites.size() - 1; i >= 0; i--){
      ClickableSprite* current_sprite = bt_panel_clickable_sprites[i];
      if (current_sprite->getGlobalBounds().contains(world_pos) && !already_hovered){
         current_sprite->setHovered(true, *this);
         already_hovered = true;
      }
      else{
         current_sprite->setHovered(false, *this);
      }
   }

   if (!already_hovered){
      this->defaultHover();
   }
}

void ParchisGUI::defaultHover(){
   this->setDefaultCursor();
}

void ParchisGUI::processEvents(){
   // Gestión de eventos (processEvents())
   Event event;
   while (this->pollEvent(event)){
      if (event.type == Event::Closed){
         this->close();
         cout << "Finalizando partida (por la fuerza)..." << endl;
         model->endGame();
         game_thread.wait();
      }

      if (event.type == Event::MouseButtonPressed || event.type == Event::MouseButtonReleased){
         // Eventos de ratón.
         //cout << pos.x << " " << pos.y << " " << world_pos.x << " " << world_pos.y << endl;
         //cout << board.getGlobalBounds().top << " " << board.getGlobalBounds().left << endl;
         bool clicked;
         if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            clicked = true;
         else
            clicked = false;

         //clicked = true;
         Vector2i pos = Mouse::getPosition(*this);
         Vector2f world_pos;

         // cout << pos.x << " " << pos.y << endl;
         //world_pos = window.mapPixelToCoords(pos);
         vector<color> colors = {red, blue, green, yellow};

         // Eventos en la vista del tablero.
         this->setView(board_view);
         world_pos = this->mapPixelToCoords(pos);

         for (int i = board_clickable_sprites.size() - 1; i >= 0; i--){
            ClickableSprite* current_sprite = board_clickable_sprites[i];
            if (clicked && current_sprite->getGlobalBounds().contains(world_pos)){
               current_sprite->setClicked(true, *this);
            }
            else{
               current_sprite->setClicked(false, *this);
            }
         }


         // Eventos en la vista de los dados.
         this->setView(dice_view);
         world_pos = this->mapPixelToCoords(pos);

         for (int i = dice_clickable_sprites.size() - 1; i >= 0; i--){
            ClickableSprite* current_sprite = dice_clickable_sprites[i];
            if (clicked && current_sprite->getGlobalBounds().contains(world_pos)){
               current_sprite->setClicked(true, *this);
            }
            else{
               current_sprite->setClicked(false, *this);
            }
         }

         for (int i = dice_dynamic_clickable_sprites.size() - 1; i >= 0; i--){
            ClickableSprite* current_sprite = dice_dynamic_clickable_sprites[i];
            if (clicked && current_sprite->getGlobalBounds().contains(world_pos)){
               current_sprite->setClicked(true, *this);
            }
            else{
               current_sprite->setClicked(false, *this);
            }
         }

         // Eventos en la vista del panel de botones.
         this->setView(bt_panel_view);
         world_pos = this->mapPixelToCoords(pos);

         for (int i = bt_panel_clickable_sprites.size() - 1; i >= 0; i--){
            ClickableSprite* current_sprite = bt_panel_clickable_sprites[i];
            if (clicked && current_sprite->getGlobalBounds().contains(world_pos)){
               current_sprite->setClicked(true, *this);
            }
            else{
               current_sprite->setClicked(false, *this);
            }
         }
      }

      if (event.type == Event::Resized){
         // resizing = true; // Variable deprecada.
         Vector2u preaspect_size = this->getSize();
         Vector2u realsize(preaspect_size.y * ASPECT_RATIO, preaspect_size.y);
         float ratio = (float)preaspect_size.x / (float)preaspect_size.y;
         float apply_ratio = ratio / 2.0f;
         float viewport_start = 0.5f - apply_ratio / 2.0f;
         float inv_ratio = 1.0f / apply_ratio;
         float inv_viewport_start = 0.5f - inv_ratio / 2.f;
         if (ratio < 2.0f){
            general_view.setViewport(FloatRect(0.f, viewport_start + apply_ratio * 0.f, 1.f, apply_ratio * 1.0f));
            board_view.setViewport(FloatRect(0.f, viewport_start + apply_ratio * 0.f, 0.5f, apply_ratio * 1.f));
            dice_view.setViewport(FloatRect(800.f / 1600.f, viewport_start + apply_ratio * 50.f / 800.f,
                                            720.f / 1600.f, apply_ratio * 320.f / 800.f));
            bt_panel_view.setViewport(FloatRect(850.f / 1600.f, viewport_start + apply_ratio * 400.f / 800.f,
                                                600.f / 1600.f, apply_ratio * 600.f / 800.f));
         }
         else{
            general_view.setViewport(FloatRect(inv_viewport_start + inv_ratio * 0.f, 0.f, inv_ratio * 1.f, 1.f));
            board_view.setViewport(FloatRect(inv_viewport_start + inv_ratio * 0.f, 0.f, inv_ratio * 0.5f, 1.f));
            dice_view.setViewport(FloatRect(inv_viewport_start + inv_ratio * 800.f / 1600.f, 50.f / 800.f,
                                            inv_ratio * 720.f / 1600.f, 320.f / 800.f));
            bt_panel_view.setViewport(FloatRect(inv_viewport_start + inv_ratio * 850.f / 1600.f, 400.f / 800.f,
                                                inv_ratio * 600.f / 1600.f, 600.f / 800.f));
         }

         //this->setSize(realsize);
      }
   }
}

void ParchisGUI::processAnimations(){
   for (int i = 0; i < all_animators.size(); i++){
      queue<shared_ptr<SpriteAnimator>>* animations_ch_i = all_animators[i];
      if (!animations_ch_i->empty()){
         //SpriteAnimator sa_i = *animations_ch_i->front();
         //sa_i.update();
         animations_ch_i->front()->update();
         if (animations_ch_i->front()->hasEnded()){
            animations_ch_i->pop();
            if (!animations_ch_i->empty()){
               animations_ch_i->front()->setStartPosition();
               animations_ch_i->front()->restart();
            }
            if (i == 0){
               void (ParchisGUI::*callback)(void) = animation_ch1_callbacks.front();
               if (callback != NULL)
                  (this->*callback)();
               animation_ch1_callbacks.pop();
            }
         }
      }
   }
}

void ParchisGUI::processSettings(){
   if (rotate_board){
      Vector2i pos = Mouse::getPosition(*this);
      FloatRect board_box = board.getGlobalBounds();
      Vector2f board_center(board_box.left + board_box.width / 2, board_box.top + board_box.height / 2);

      float angle = atan2(pos.x - board_center.x, pos.y - board_center.y) * 180.f / PI;

      board_view.rotate(angle - rotate_angle0);
      rotate_angle0 = angle;
   }
   // Keep aspect ratio.
   //Vector2u preaspect_size = this->getSize();
   //Vector2u realsize(preaspect_size.y * ASPECT_RATIO, preaspect_size.y);
   //this->setSize(realsize);
}

void ParchisGUI::paint(){
   this->clear(Color::White);

   //Dibujamos elementos generales (sin vistas)
   this->setView(general_view);
   for (int i = 0; i < general_drawable_sprites.size(); i++){
      this->draw(*general_drawable_sprites[i]);
   }
   star_shader.setUniform("u_resolution", sf::Glsl::Vec2{this->getSize()});
   star_shader.setUniform("u_mouse", sf::Glsl::Vec2{sf::Vector2f{}});
   star_shader.setUniform("u_time", global_clock.getElapsedTime().asSeconds());
   star_shader.setUniform("texture", sf::Shader::CurrentTexture);
   boo_shader.setUniform("u_resolution", sf::Glsl::Vec2{this->getSize()});
   boo_shader.setUniform("u_mouse", sf::Glsl::Vec2{sf::Vector2f{}});
   boo_shader.setUniform("u_time", global_clock.getElapsedTime().asSeconds());
   boo_shader.setUniform("texture", sf::Shader::CurrentTexture);
   star_boom_shader.setUniform("u_resolution", sf::Glsl::Vec2{this->getSize()});
   star_boom_shader.setUniform("u_mouse", sf::Glsl::Vec2{sf::Vector2f{}});
   star_boom_shader.setUniform("u_time", global_clock.getElapsedTime().asSeconds());
   star_boom_shader.setUniform("texture", sf::Shader::CurrentTexture);
   bananed_shader.setUniform("u_time", global_clock.getElapsedTime().asSeconds());
   bananed_shader.setUniform("u_resolution", sf::Glsl::Vec2{this->getSize()});
   dice_shader.setUniform("u_time", global_clock.getElapsedTime().asSeconds());
   dice_shader.setUniform("u_resolution", sf::Glsl::Vec2{this->getSize()});

   //Dibujamos elementos de la vista del tablero.
   this->setView(board_view);
   this->draw(*board_drawable_sprites[0]);
   for (int i = 0; i < board_dynamic_drawable_sprites.size(); i++){
      this->draw(*board_dynamic_drawable_sprites[i]);
   }
   for (int i = 1; i < board_drawable_sprites.size(); i++){
      if (piece_sprite_start <= i and i < piece_sprite_end){
         PieceSprite* ps = static_cast<PieceSprite*>(board_drawable_sprites[i]);
         switch (ps->getPiece().get_type()){
         case star_piece:
            star_shader.setUniform("sfmlColor",
                                   sf::Glsl::Vec4(board_drawable_sprites[i]->getColor().r / 255.f,
                                                  board_drawable_sprites[i]->getColor().g / 255.f,
                                                  board_drawable_sprites[i]->getColor().b / 255.f,
                                                  board_drawable_sprites[i]->getColor().a / 255.f));
            this->draw(*board_drawable_sprites[i], &star_shader);
            break;
         case boo_piece:
            boo_shader.setUniform("sfmlColor",
                                  sf::Glsl::Vec4(board_drawable_sprites[i]->getColor().r / 255.f,
                                                 board_drawable_sprites[i]->getColor().g / 255.f,
                                                 board_drawable_sprites[i]->getColor().b / 255.f,
                                                 board_drawable_sprites[i]->getColor().a / 255.f));
            this->draw(*board_drawable_sprites[i], &boo_shader);
            break;
         case bananed_piece:
            bananed_shader.setUniform("sfmlColor",
                                      sf::Glsl::Vec4(board_drawable_sprites[i]->getColor().r / 255.f,
                                                     board_drawable_sprites[i]->getColor().g / 255.f,
                                                     board_drawable_sprites[i]->getColor().b / 255.f,
                                                     board_drawable_sprites[i]->getColor().a / 255.f));
            this->draw(*board_drawable_sprites[i], &bananed_shader);
            break;
         case normal_piece:
         default:
            this->draw(*board_drawable_sprites[i]);
            break;
         }
      }
      else if (golden_boom_sprite_start <= i and i < golden_boom_sprite_end){
         star_boom_shader.setUniform("sfmlColor",
                                     sf::Glsl::Vec4(board_drawable_sprites[i]->getColor().r / 255.f,
                                                    board_drawable_sprites[i]->getColor().g / 255.f,
                                                    board_drawable_sprites[i]->getColor().b / 255.f,
                                                    board_drawable_sprites[i]->getColor().a / 255.f));
         this->draw(*board_drawable_sprites[i], &star_boom_shader);
      }
      else{
         this->draw(*board_drawable_sprites[i]);
      }
      /*
      PieceSprite *ps = dynamic_cast<PieceSprite*>(board_drawable_sprites[i]);
      if(ps == NULL or ps->getPiece().get_type() == normal_piece)
          this->draw(*board_drawable_sprites[i]);
      else{
          switch(ps->getPiece().get_type()){
              case star_piece:
                  star_shader.setUniform("sfmlColor", sf::Glsl::Vec4(board_drawable_sprites[i]->getColor().r / 255.f, board_drawable_sprites[i]->getColor().g / 255.f, board_drawable_sprites[i]->getColor().b / 255.f, board_drawable_sprites[i]->getColor().a / 255.f));
                  this->draw(*board_drawable_sprites[i], &star_shader);
              break;
              case boo_piece:
                  boo_shader.setUniform("sfmlColor", sf::Glsl::Vec4(board_drawable_sprites[i]->getColor().r / 255.f, board_drawable_sprites[i]->getColor().g / 255.f, board_drawable_sprites[i]->getColor().b / 255.f, board_drawable_sprites[i]->getColor().a / 255.f));
                  this->draw(*board_drawable_sprites[i], &boo_shader);
              break;
          }

      }*/
   }


   // Dibujamos elementos de la vista de los dados.
   this->setView(dice_view);
   for (int i = 0; i < dice_drawable_sprites.size(); i++){
      if (dice_sprite_start <= i and i < dice_sprite_end){
         DiceSprite* ds = static_cast<DiceSprite*>(dice_drawable_sprites[i]);
         ds->setShaderColors(dice_shader);
         this->draw(*dice_drawable_sprites[i], &dice_shader);
      }
      else if (i == turns_arrow_sprite_pos){
         Color actual_colorA = DiceSprite::color2Color.at(model->getCurrentColor());
         Color actual_colorB = actual_colorA; // DiceSprite::color2Color.at(partner_color(model->getCurrentColor()));
         dice_shader.setUniform(
            "colorA", sf::Glsl::Vec3(actual_colorA.r / 255.0, actual_colorA.g / 255.0, actual_colorA.b / 255.0));
         dice_shader.setUniform(
            "colorB", sf::Glsl::Vec3(actual_colorB.r / 255.0, actual_colorB.g / 255.0, actual_colorB.b / 255.0));
         this->draw(*dice_drawable_sprites[i], &dice_shader);
      }
      else
         this->draw(*dice_drawable_sprites[i]);
   }

   for (int i = 0; i < dice_dynamic_drawable_sprites.size(); i++){
      if (dynamic_dice_sprite_start <= i and i < dynamic_dice_sprite_end){
         DiceSprite* ds = static_cast<DiceSprite*>(dice_dynamic_drawable_sprites[i]);
         ds->setShaderColors(dice_shader);
         this->draw(*dice_dynamic_drawable_sprites[i], &dice_shader);
      }
      else
         this->draw(*dice_dynamic_drawable_sprites[i]);
   }

   // Dibujamos elementos de la vista de los botones
   this->setView(bt_panel_view);
   for (int i = 0; i < bt_panel_drawable_sprites.size(); i++){
      this->draw(*bt_panel_drawable_sprites[i]);
   }

   this->display();
}

void ParchisGUI::updateSprites(){
   checkSwitchMusic();
   vector<color> colors = Parchis::game_colors;
   if (model->isEatingMove()){
      cout << "TOCA CONTARSE 20" << endl;
      this->last_dice = 20;
   }
   if (model->isGoalMove()){
      cout << "TOCA CONTARSE 10" << endl;
      this->last_dice = 10;
      //checkHurryUp();
   }

   // cout << "last_dice: " << last_dice << endl;

   if (model->getCurrentPlayer().canUseGUI()){
      this->notPlayableLock(false);
   }
   else{
      this->notPlayableLock(true);
   }

   if (model->isPlaygroundMode()){
      this->auto_heuristic_button.setEnabled(false, *this);
      this->auto_heuristic_button.setLocked(true, *this);
      this->auto_heuristic_button.setSelected(false, *this);
      this->move_heuristic_button.setEnabled(false, *this);
      this->move_heuristic_button.setLocked(true, *this);
   }

   bool def_lock = animation_lock || not_playable_lock;

   vector<color> dice_colors = {yellow, blue};
   for (int i = 0; i < dice_colors.size(); i++){
      color c = dice_colors[i];
      Dice dice = model->getDice();
      for (int j = 0; j < this->dices[c].size(); j++){
         DiceSprite* current = &this->dices[c][j];
         if (this->last_dice == 10 || this->last_dice == 20){
            current->setLocked(true, *this);
            current->setSelected(false, *this);
            current->setEnabled(dice.isAvailable(c, current->getNumber()), *this);
         }
         else{
            if (animation_lock){
               current->setLocked(true, *this);
               color last_col = get<0>(model->getLastAction());
               int last_move_dice = get<2>(model->getLastAction());
               current->setEnabled(
                  (c == last_col && current->getNumber() == last_move_dice) || dice.isAvailable(
                     c, current->getNumber()), *this);
               current->setSelected(c == last_col && current->getNumber() == last_move_dice, *this);
            }

            else{
               current->setEnabled(dice.isAvailable(c, current->getNumber()), *this);
               current->setLocked(this->model->getCurrentMainColor() != c || def_lock, *this);
               current->setSelected(this->model->getCurrentMainColor() == c and last_dice == current->getNumber(),
                                    *this);
            }
         }
      }

      for (int j = 0; j < this->special_dices[c].size(); j++){
         DiceSprite* current = &this->special_dices[c][j];
         if (this->last_dice == 10 || this->last_dice == 20){
            current->setLocked(true, *this);
            current->setSelected(false, *this);
            current->setEnabled(dice.isAvailable(c, current->getNumber()), *this);
         }
         else{
            if (animation_lock){
               current->setLocked(true, *this);
               color last_col = get<0>(model->getLastAction());
               int last_move_dice = get<2>(model->getLastAction());
               current->setEnabled(
                  (c == last_col && current->getNumber() == last_move_dice) || dice.isAvailable(
                     c, current->getNumber()), *this);
               current->setSelected(c == last_col && current->getNumber() == last_move_dice, *this);
            }

            else{
               current->setEnabled(dice.isAvailable(c, current->getNumber()), *this);
               current->setLocked(this->model->getCurrentMainColor() != c || def_lock, *this);
               current->setSelected(this->model->getCurrentMainColor() == c and last_dice == current->getNumber(),
                                    *this);
            }
         }
      }

      // Activar dados especiales para las comidas y las metas.
      if (model->isEatingMove() && c == model->getCurrentMainColor()){
         special_10_20_dice[c][0].setEnabled(true, *this);
         special_10_20_dice[c][0].setLocked(false || def_lock, *this);
         special_10_20_dice[c][0].setSelected(true, *this);
         special_10_20_dice[c][0].setNumber(20);
      }
      else if (model->isGoalMove() && c == model->getCurrentMainColor()){
         special_10_20_dice[c][0].setEnabled(true, *this);
         special_10_20_dice[c][0].setLocked(false || def_lock, *this);
         special_10_20_dice[c][0].setSelected(true, *this);
         special_10_20_dice[c][0].setNumber(10);
      }
      else{
         special_10_20_dice[c][0].setEnabled(false, *this);
         special_10_20_dice[c][0].setLocked(true, *this);
         special_10_20_dice[c][0].setSelected(false, *this);
         special_10_20_dice[c][0].setNumber(-1);
      }
   }


   for (int i = 0; i < colors.size(); i++){
      color c = colors[i];
      vector<Piece> player_pieces = model->getBoard().getPieces(c);
      vector<Piece> partner_pieces = model->getBoard().getPieces(partner_color(c));
      if (this->model->getCurrentColor() == c || this->model->getCurrentColor() == partner_color(c)){
         for (int j = 0; j < player_pieces.size(); j++){
            this->pieces[c][j].setEnabled(model->isLegalMove(player_pieces[j], last_dice), *this);
            this->pieces[c][j].setLocked(!model->isLegalMove(player_pieces[j], last_dice) || def_lock, *this);
         }

         for (int j = 0; j < partner_pieces.size(); j++){
            this->pieces[partner_color(c)][j].setEnabled(model->isLegalMove(partner_pieces[j], last_dice), *this);
            this->pieces[partner_color(c)][j].setLocked(
               !model->isLegalMove(partner_pieces[j], last_dice) || def_lock, *this);
         }
      }
      else{
         for (int j = 0; j < player_pieces.size(); j++){
            this->pieces[c][j].setEnabled(true, *this);
            this->pieces[c][j].setLocked(true, *this);
         }

         for (int j = 0; j < partner_pieces.size(); j++){
            this->pieces[partner_color(c)][j].setEnabled(true, *this);
            this->pieces[partner_color(c)][j].setLocked(true, *this);
         }
      }
   }

   if (!this->animation_lock){
      vector<color> colors = {yellow, blue, red, green};
      // Actualizar estado de las fichas que pierden su efecto especial.
      for (int i = 0; i < colors.size(); i++){
         color col = colors[i];
         for (int j = 0; j < pieces.at(col).size(); j++){
            setPieceAttributesOnBoard(pieces[col][j], j, 0);
            Vector2f pos = getPiecePositionOnBoard(pieces[col][j], j, 0);
            pieces[col][j].setPosition(pos.x, pos.y);
         }
      }
   }

   // Actualizar color y disponibilidad del botón de pasar turno.
   this->skip_turn_button.setModelColor(model->getCurrentColor());
   this->skip_turn_button.setEnabled(model->canSkipTurn(model->getCurrentMainColor(), last_dice), *this);
   this->skip_turn_button.setLocked(!model->canSkipTurn(model->getCurrentMainColor(), last_dice) || def_lock, *this);
}

void ParchisGUI::updateSpritesLock(){
   if (animation_lock || not_playable_lock){
      //Por algún motivo la aplicación a veces se cuelga cuando se cambian estos cursores.
      //this->setDefaultCursorThinking(); // Este cursor hace que el programa se rompa, pero solo cuando se pone aquí ???
      //this->setDefaultCursorForbidden();
      //this->setDefaultCursor();

      vector<color> colors = Parchis::game_colors;
      vector<color> dice_colors = {yellow, blue};

      for (int i = 0; i < dice_colors.size(); i++){
         color c = dice_colors[i];
         for (int j = 0; j < this->dices[c].size(); j++){
            DiceSprite* current = &this->dices[c][j];
            current->setLocked(true, *this);
         }

         special_10_20_dice[c][0].setLocked(true, *this);
      }

      for (int i = 0; i < colors.size(); i++){
         color c = colors[i];
         color partner_c = partner_color(c);
         vector<Piece> player_pieces = model->getBoard().getPieces(c);
         vector<Piece> partner_pieces = model->getBoard().getPieces(partner_c);
         for (int j = 0; j < player_pieces.size(); j++){
            this->pieces[c][j].setLocked(true, *this);
         }

         for (int j = 0; j < partner_pieces.size(); j++){
            this->pieces[partner_c][j].setLocked(true, *this);
         }
      }

      this->skip_turn_button.setLocked(true, *this);

      this->move_heuristic_button.setEnabled(false, *this);
      this->move_heuristic_button.setLocked(true, *this);
   }
   else if (!model->isPlaygroundMode()){
      this->move_heuristic_button.setEnabled(true, *this);
      this->move_heuristic_button.setLocked(false, *this);
   }
}


void ParchisGUI::animationLock(bool lock){
   mutex.lock();
   if (lock != animation_lock){
      this->animation_lock = lock;
      updateSpritesLock();
   }
   mutex.unlock();
}

void ParchisGUI::notPlayableLock(bool lock){
   mutex.lock();
   if (lock != not_playable_lock){
      this->not_playable_lock = lock;
      updateSpritesLock();
   }
   mutex.unlock();
}

bool ParchisGUI::animationsRunning(){
   return !this->animations_ch1.empty() || !this->animations_ch2.empty() || !this->animations_ch3.empty() || !this->
      animations_ch4.empty();
}

void ParchisGUI::run(){
   while (this->isOpen()){
      mainLoop();
   }
}

//col_pieces_sprites[j].setPosition((Vector2f)box2position.at(model.getBoard().getPiece(col, j))[j]);


Vector2f ParchisGUI::box3position(color c, int id, int pos){
   Box piece = model->getBoard().getPiece(c, id).get_box();
   if (piece.type == home || piece.type == goal){
      return (Vector2f)box2position.at(piece)[id];
   }
   else{
      if (model->boxState(piece).size() == 1)
         return (Vector2f)box2position.at(piece)[pos];
      // Realmente pos no haría falta para nada, se podría deprecar, pero paso por ahora xd
      else{
         vector<pair<color, int>> box_state = model->boxState(piece);
         pair<color, int> piece0 = box_state[0];
         pair<color, int> piece1 = box_state[1];
         if (piece0.first == c && piece0.second == id)
            return (Vector2f)box2position.at(piece)[1];
         else if (piece1.first == c && piece1.second == id)
            return (Vector2f)box2position.at(piece)[2];
         else{
            cout << "Posible error en box3position????" << endl;
            return (Vector2f)box2position.at(piece)[pos];
         }
      }
   }
}

Vector2f ParchisGUI::box3position(Box piece, int id, int pos){
   if (piece.type == home || piece.type == goal){
      return (Vector2f)box2position.at(piece)[id];
   }
   else{
      return (Vector2f)box2position.at(piece)[pos];
   }
}

Vector2f ParchisGUI::getPiecePositionOnBoard(const PieceSprite& ps, int id, int pos){
   Piece p = ps.getPiece();
   color col = p.get_color();
   if (p.get_type() == small_piece){
      return box3position(col, id, 0) + Vector2f(15, 15);
   }
   else if (p.get_type() == mega_piece){
      Vector2f pos1 = box3position(col, id, 0);
      Box next_box = this->model->nextBox(p);
      Vector2f pos2 = box3position(next_box, id, 0);
      Vector2f middle_origin = (pos1 + pos2) / 2.f;
      return middle_origin + Vector2f(15, 15);
   }
   else{
      return box3position(col, id, pos) + Vector2f(15, 15);
   }

   /*
   Vector2f middle_point;
   middle_point = (box3position(col, j, 0) + box3position(col, j % 68, 0)) / 2.f;
   //col_pieces_sprites[j].setPosition(middle_point);
   col_pieces_sprites[j].setOrigin(15, 15);
   col_pieces_sprites[j].setScale(2.0, 2.0);
   */

   /*
   col_pieces_sprites[j].setPosition(box3position(col, j, 0) + Vector2f(15, 15));
   col_pieces_sprites[j].setOrigin(15, 15);
   col_pieces_sprites[j].setScale(0.5, 0.5);
   */
}

Vector2f ParchisGUI::getPiecePositionOnBoard(const PieceSprite& ps, const Box& box, int pos){
   Piece p = ps.getPiece();
   color col = p.get_color();
   if (p.get_type() == small_piece){
      return (Vector2f)box2position.at(box).at(pos) + Vector2f(15, 15);
   }
   else if (p.get_type() == mega_piece){
      Vector2f pos1 = (Vector2f)box2position.at(box).at(0);
      Box next_box = this->model->nextBox(p);
      Vector2f pos2 = (Vector2f)box2position.at(next_box).at(0);
      Vector2f middle_origin = (pos1 + pos2) / 2.f;
      return middle_origin + Vector2f(15, 15);
   }
   else{
      return (Vector2f)box2position.at(box).at(pos) + Vector2f(15, 15);
   }
}

void ParchisGUI::setPieceAttributesOnBoard(PieceSprite& ps, int id, int pos, int anim_time, int max_time){
   float new_scale;
   if (animation_time > 1000) anim_time = 1000;
   Piece p = ps.getPiece();
   color col = p.get_color();
   if (p.get_type() == small_piece){
      ps.setOrigin(15, 15);
      new_scale = 0.5;
   }
   else if (p.get_type() == mega_piece){
      ps.setOrigin(15, 15);
      new_scale = 2.0;
   }
   else{
      ps.setOrigin(15, 15);
      new_scale = 1.0;
   }

   ps.setScale(new_scale, new_scale);

   // From current ps.getScale() to new_scale in anim_time from 0 to max_time
   //if(new_scale != ps.getScale().x){
   //    float curr_scale = (max_time - anim_time) * ps.getScale().x / max_time + anim_time * new_scale / max_time;
   //    ps.setScale(new_scale, new_scale);
   //}

   /*
   Vector2f middle_point;
   middle_point = (box3position(col, j, 0) + box3position(col, j % 68, 0)) / 2.f;
   //col_pieces_sprites[j].setPosition(middle_point);
   col_pieces_sprites[j].setOrigin(15, 15);
   col_pieces_sprites[j].setScale(2.0, 2.0);
   */

   /*
   col_pieces_sprites[j].setPosition(box3position(col, j, 0) + Vector2f(15, 15));
   col_pieces_sprites[j].setOrigin(15, 15);
   col_pieces_sprites[j].setScale(0.5, 0.5);
   */
}

//Cursores
void ParchisGUI::setDefaultCursor(){
   //if(cursor.loadFromSystem(Cursor::Arrow))
   //    this->setMouseCursor(cursor);
   mutex.lock();
   if (animation_lock || not_playable_lock)
      this->setThinkingCursor();
   else
      this->setNormalCursor();
   //this->setMouseCursor(default_cursor);
   mutex.unlock();
}

void ParchisGUI::setNormalCursor(){
   mutex.lock();
   if (cursor.loadFromSystem(Cursor::Arrow))
      this->setMouseCursor(cursor);
   mutex.unlock();
}

void ParchisGUI::setForbiddenCursor(){
   mutex.lock();
   if (cursor.loadFromSystem(Cursor::NotAllowed))
      this->setMouseCursor(cursor);
   mutex.unlock();
}

void ParchisGUI::setHandCursor(){
   mutex.lock();
   if (cursor.loadFromSystem(Cursor::Hand))
      this->setMouseCursor(cursor);
   mutex.unlock();
}

void ParchisGUI::setThinkingCursor(){
   //mutex.lock();
   //if (cursor.loadFromSystem(Cursor::Wait))
   //    this->setMouseCursor(cursor);
   //mutex.unlock();
}

void ParchisGUI::setDefaultCursorNormal(){
   mutex.lock();
   default_cursor.loadFromSystem(Cursor::Arrow);
   mutex.unlock();
}

void ParchisGUI::setDefaultCursorForbidden(){
   mutex.lock();
   default_cursor.loadFromSystem(Cursor::NotAllowed);
   mutex.unlock();
}

void ParchisGUI::setDefaultCursorHand(){
   mutex.lock();
   default_cursor.loadFromSystem(Cursor::Hand);
   mutex.unlock();
}

void ParchisGUI::setDefaultCursorThinking(){
   //mutex.lock();
   //default_cursor.loadFromSystem(Cursor::Wait);
   //mutex.unlock();
}


void ParchisGUI::queueMove(color col, int id, Box origin, Box dest, void (ParchisGUI::*callback)(void)){
   this->animationLock(true);
   animation_ch1_callbacks.push(callback);
   if (dest.type == home || dest.type == goal){
      // Si el destino es casa o meta cada ficha va a su puesto preasignado por id.
      Vector2f animate_pos = getPiecePositionOnBoard(pieces[col][id], dest, id);
      //(Vector2f)box2position.at(dest)[id];

      Sprite* animate_sprite = &pieces[col][id];
      shared_ptr<SpriteAnimator> animator = make_shared<SpriteAnimator>(*animate_sprite, animate_pos, animation_time);
      animations_ch1.push(animator);

      // Si el destino es casa y hay algún flag de red_shell/blue_shell/star move activo, se encola la explosión.
      if (dest.type == home){
         if (model->isRedShellMove() or model->isBlueShellMove() or model->isStarMove() or model->isHornMove()){
            Vector2f animate_pos = (Vector2f)box2position.at(dest)[id];
            Sprite* animate_sprite;
            if (model->isRedShellMove())
               animate_sprite = &red_boom[current_boom_sprite];
            else if (model->isBlueShellMove())
               animate_sprite = &blue_boom[current_boom_sprite];
            else if (model->isStarMove())
               animate_sprite = &golden_boom[current_boom_sprite];
            else if (model->isHornMove())
               animate_sprite = &horn_boom[current_boom_sprite];

            if (!model->isHornMove()){
               current_boom_sprite = (current_boom_sprite + 1) % BOOM_SPRITE_LIMIT;
               animate_pos = (Vector2f)box2position.at(origin)[0] + Vector2f(
                  animate_sprite->getLocalBounds().width / 2, animate_sprite->getLocalBounds().height / 2);
               animate_sprite->setPosition(animate_pos);
               animate_sprite->setOrigin(animate_sprite->getLocalBounds().width / 2,
                                         animate_sprite->getLocalBounds().height / 2);
               shared_ptr<ExplosionAnimator> animator = make_shared<ExplosionAnimator>(
                  *animate_sprite, 1.f, 3.f, animation_time);
               animations_ch5.push(animator);
            }
            else{
               current_boom_sprite = (current_boom_sprite + 1) % BOOM_SPRITE_LIMIT;
               animate_pos = (Vector2f)box2position.at(origin)[0] + Vector2f(
                  animate_sprite->getLocalBounds().width / 2, animate_sprite->getLocalBounds().height / 2);
               animate_sprite->setPosition(animate_pos);
               animate_sprite->setOrigin(animate_sprite->getLocalBounds().width / 2,
                                         animate_sprite->getLocalBounds().height / 2);
               shared_ptr<ExplosionAnimator> animator = make_shared<ExplosionAnimator>(
                  *animate_sprite, 1.f, 3.f, animation_time);
               animations_ch5.push(animator);
            }
         }
      }
   }

   // else if(model->isHornMove()){
   //     // La ficha que ha pegado el bocinazo genera una explosión.
   //     Vector2f animate_pos = (Vector2f)box2position.at(origin)[0] + Vector2f(horn_boom.getLocalBounds().width/2, horn_boom.getLocalBounds().height/2);
   //     horn_boom.setPosition(animate_pos);
   //     horn_boom.setOrigin(horn_boom.getLocalBounds().width/2, horn_boom.getLocalBounds().height/2);
   //     shared_ptr<ExplosionAnimator> animator = make_shared<ExplosionAnimator>(horn_boom, 1.f, 6.f, animation_time);
   //     animations_ch5.push(animator);
   //     playHornSound();
   // }
   else{
      // Buscamos colisiones.
      vector<pair<color, int>> occupation = this->model->boxState(dest);
      if (occupation.size() < 2){
         // Si no había fichas en destino se mueve la ficha al sitio central.
         Vector2f animate_pos = getPiecePositionOnBoard(pieces[col][id], dest, 0);
         //(Vector2f)box2position.at(dest)[0];

         Sprite* animate_sprite = &pieces[col][id];
         shared_ptr<SpriteAnimator> animator = make_shared<SpriteAnimator>(
            *animate_sprite, animate_pos, animation_time);
         animations_ch1.push(animator);
      }
      else if (occupation.size() == 2){
         // Si hay dos fichas en destino mandamos cada una a un lateral.
         int main_move = (occupation[0].first == col && occupation[0].second == id) ? 0 : 1;
         int collateral_move = (main_move == 0) ? 1 : 0;

         // Ficha principal (la que realmente se mueve) por el canal 1 por si hay que encadenar animaciones.
         Vector2f animate_pos = getPiecePositionOnBoard(pieces[col][id], dest, 1);
         //(Vector2f)box2position.at(dest)[1];
         Sprite* animate_sprite = &pieces[occupation[main_move].first][occupation[main_move].second];
         shared_ptr<SpriteAnimator> animator = make_shared<SpriteAnimator>(
            *animate_sprite, animate_pos, animation_time);
         animations_ch1.push(animator);

         // Ficha desplazada por el canal 2.
         Vector2f animate_pos2 = getPiecePositionOnBoard(pieces[col][id], dest, 2);
         //(Vector2f)box2position.at(dest)[2];
         Sprite* animate_sprite2 = &pieces[occupation[collateral_move].first][occupation[collateral_move].second];
         shared_ptr<SpriteAnimator> animator2 = make_shared<SpriteAnimator>(
            *animate_sprite2, animate_pos2, animation_time);
         animations_ch2.push(animator2);
      }
   }

   if (origin.type != goal && origin.type != home){
      vector<pair<color, int>> origin_occupation = this->model->boxState(origin);
      if (origin_occupation.size() == 1 && !model->isStarMove()){
         // Si queda una ficha en el origen del movimiento tras haber hecho el movimiento, la devolvemos al centro (canal 3).
         // (Siempre que el origen no sea ni casa ni meta).
         Vector2f animate_pos = getPiecePositionOnBoard(
            pieces[origin_occupation.at(0).first][origin_occupation.at(0).second], origin,
            0); //(Vector2f)box2position.at(origin)[0];
         Sprite* animate_sprite = &pieces[origin_occupation.at(0).first][origin_occupation.at(0).second];
         shared_ptr<SpriteAnimator> animator = make_shared<SpriteAnimator>(
            *animate_sprite, animate_pos, animation_time);
         animations_ch3.push(animator);
      }
   }

   if (model->isStarMove() && dest.type != home){
      // Si está activo el flag de star move pero no se manda una ficha a casa, se encola una animación vacía por el canal de explosiones.
      shared_ptr<ExplosionAnimator> animator = make_shared<ExplosionAnimator>(1.f, 3.f, animation_time);
      animations_ch5.push(animator);
   }
}

void ParchisGUI::queueTurnsArrow(color c){
   this->animationLock(true);
   // Actualizar posición y color de la flecha de turnos.
   int new_turn_pos = color2turns_arrow_pos.at(c);
   if (new_turn_pos != turns_arrow.getPosition().y){
      shared_ptr<SpriteAnimator> s = make_shared<SpriteAnimator>(turns_arrow,
                                                                 Vector2f(turns_arrow.getPosition().x, new_turn_pos),
                                                                 animation_time);
      animations_ch4.push(s);
   }
   Color turns_arrow_color = turns_arrow.getColor();
   if (turns_arrow_color != DiceSprite::color2Color.at(c)){
      turns_arrow.setColor(DiceSprite::color2Color.at(c));
   }
}

void ParchisGUI::setBackgroundMusic(bool on){
   music_on = on;
   if (on){
      current_background_theme->play();
   }
   else{
      current_background_theme->stop();
   }
}

void ParchisGUI::initializeBackgroundMusic(){
   // Main background theme.
   if (background_theme.openFromFile(background_theme_file + ".wav")){
      background_theme.setLoop(true); // Para reproducir en bucle.

      ifstream loop_file((background_theme_file + ".loop").c_str());
      if (loop_file.good()){
         float loop_start, loop_end;
         loop_file >> loop_start;
         loop_file >> loop_end;
         background_theme.setLoopPoints(Music::TimeSpan(seconds(loop_start), seconds(loop_end - loop_start)));
         // Se puede elegir los puntos exactos en los que cicle la música de fondo.
         cout << "Added loop points for background theme: " << loop_start << " " << loop_end << endl;
      }
      else
         cout << "No loop points found for background theme." << endl;

      background_theme.setVolume(100.f);
   }
   // Hurry up background theme.
   if (background_theme_hurryup.openFromFile(background_theme_hurryup_file + ".wav")){
      background_theme_hurryup.setLoop(true);
      background_theme_hurryup.setVolume(100.f);

      ifstream loop_file((background_theme_hurryup_file + ".loop").c_str());
      if (loop_file.good()){
         float loop_start, loop_end;
         loop_file >> loop_start;
         loop_file >> loop_end;
         background_theme_hurryup.
            setLoopPoints(Music::TimeSpan(seconds(loop_start), seconds(loop_end - loop_start)));
         cout << "Added loop points for hurry up background theme: " << loop_start << " " << loop_end << endl;
      }
      else
         cout << "No loop points found for hurry up background theme." << endl;
   }
   // Victory theme.
   if (background_theme_win.openFromFile(background_theme_win_file + ".wav")){
      background_theme_win.setVolume(100.f);
   }
   // Defeat theme.
   if (background_theme_lose.openFromFile(background_theme_lose_file + ".wav")){
      background_theme_lose.setVolume(100.f);
   }
   // Star theme.
   if (background_theme_star.openFromFile(background_theme_star_file + ".wav")){
      background_theme_star.setLoop(true);
      background_theme_star.setVolume(100.f);

      ifstream loop_file((background_theme_star_file + ".loop").c_str());
      if (loop_file.good()){
         float loop_start, loop_end;
         loop_file >> loop_start;
         loop_file >> loop_end;
         background_theme_star.setLoopPoints(Music::TimeSpan(seconds(loop_start), seconds(loop_end - loop_start)));
         cout << "Added loop points for star background theme: " << loop_start << " " << loop_end << endl;
      }
      else
         cout << "No loop points found for star background theme." << endl;
   }
   // Mega theme.
   if (background_theme_mega.openFromFile(background_theme_mega_file + ".wav")){
      background_theme_mega.setLoop(true);
      background_theme_mega.setVolume(100.f);

      ifstream loop_file((background_theme_mega_file + ".loop").c_str());
      if (loop_file.good()){
         float loop_start, loop_end;
         loop_file >> loop_start;
         loop_file >> loop_end;
         background_theme_mega.setLoopPoints(Music::TimeSpan(seconds(loop_start), seconds(loop_end - loop_start)));
         cout << "Added loop points for mega background theme: " << loop_start << " " << loop_end << endl;
      }
      else
         cout << "No loop points found for mega background theme." << endl;
   }
   // Shock theme.
   if (background_theme_shock.openFromFile(background_theme_shock_file + ".wav")){
      background_theme_shock.setLoop(true);
      background_theme_shock.setVolume(100.f);

      ifstream loop_file((background_theme_shock_file + ".loop").c_str());
      if (loop_file.good()){
         float loop_start, loop_end;
         loop_file >> loop_start;
         loop_file >> loop_end;
         background_theme_shock.setLoopPoints(Music::TimeSpan(seconds(loop_start), seconds(loop_end - loop_start)));
         cout << "Added loop points for shock background theme: " << loop_start << " " << loop_end << endl;
      }
      else
         cout << "No loop points found for shock background theme." << endl;
   }

   current_background_theme = &background_theme;
}

void ParchisGUI::switchBackgroundMusic(){
   current_background_theme->stop();
   if (current_background_theme == &background_theme){
      current_background_theme = &background_theme_hurryup;
   }
   else{
      current_background_theme = &background_theme;
   }
   if (music_on){
      current_background_theme->play();
   }
}

void ParchisGUI::switchBackgroundMusic(Music& m){
   if (current_background_theme == &m){
      return;
   }
   if (current_background_theme == &background_theme and &m == &background_theme_shock or
      current_background_theme == &background_theme_shock and &m == &background_theme){
      m.setPlayingOffset(current_background_theme->getPlayingOffset());
   }
   if (current_background_theme != nullptr)
      current_background_theme->stop();

   current_background_theme = &m;
   if (music_on){
      current_background_theme->play();
   }
}

void ParchisGUI::initializeSoundEffects(){
   if (sound_buffer_move.loadFromFile(sound_move_file + ".wav")){
      sound_move.setBuffer(sound_buffer_move);
      sound_move.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_move_file << endl;
   }
   if (sound_buffer_boing.loadFromFile(sound_boing_file + ".wav")){
      sound_boing.setBuffer(sound_buffer_boing);
      sound_boing.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_boing_file << endl;
   }
   if (sound_buffer_forbidden.loadFromFile(sound_forbidden_file + ".wav")){
      sound_forbidden.setBuffer(sound_buffer_forbidden);
      sound_forbidden.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_forbidden_file << endl;
   }
   if (sound_buffer_eaten.loadFromFile(sound_eaten_file + ".wav")){
      sound_eaten.setBuffer(sound_buffer_eaten);
      sound_eaten.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_eaten_file << endl;
   }
   if (sound_buffer_applause.loadFromFile(sound_applause_file + ".wav")){
      sound_applause.setBuffer(sound_buffer_applause);
      sound_applause.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_applause_file << endl;
   }
   if (sound_buffer_explosion.loadFromFile(sound_explosion_file + ".wav")){
      sound_explosion.setBuffer(sound_buffer_explosion);
      sound_explosion.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_explosion_file << endl;
   }
   if (sound_buffer_starhit.loadFromFile(sound_starhit_file + ".wav")){
      sound_starhit.setBuffer(sound_buffer_starhit);
      sound_starhit.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_starhit_file << endl;
   }
   if (sound_buffer_shock.loadFromFile(sound_shock_file + ".wav")){
      sound_shock.setBuffer(sound_buffer_shock);
      sound_shock.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_shock_file << endl;
   }
   if (sound_buffer_boo.loadFromFile(sound_boo_file + ".wav")){
      sound_boo.setBuffer(sound_buffer_boo);
      sound_boo.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_boo_file << endl;
   }
   if (sound_buffer_horn.loadFromFile(sound_horn_file + ".wav")){
      sound_horn.setBuffer(sound_buffer_horn);
      sound_horn.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_horn_file << endl;
   }
   if (sound_buffer_bullet.loadFromFile(sound_bullet_file + ".wav")){
      sound_bullet.setBuffer(sound_buffer_bullet);
      sound_bullet.setVolume(100.f);
      cout << "Loaded sound effect: " << sound_bullet_file << endl;
   }
}

void ParchisGUI::setSoundEffects(bool on){
   this->sound_on = on;
}

void ParchisGUI::playMoveSound(){
   if (sound_on){
      //Sound sound_move;
      //sound_move.setBuffer(sound_buffer_move);
      sound_move.play();
   }
}

void ParchisGUI::playBoingSound(){
   if (sound_on){
      //Sound sound_boing;
      //sound_boing.setBuffer(sound_buffer_boing);
      sound_boing.play();
   }
}

void ParchisGUI::playForbiddenSound(){
   if (sound_on){
      //Sound sound_forbidden;
      //sound_forbidden.setBuffer(sound_buffer_forbidden);
      sound_forbidden.play();
   }
}

void ParchisGUI::playEatenSound(){
   if (sound_on){
      //Sound sound_eaten;
      //sound_eaten.setBuffer(sound_buffer_eaten);
      sound_eaten.play();
   }
}

void ParchisGUI::playApplauseSound(){
   if (sound_on){
      //Sound sound_applause;
      //sound_applause.setBuffer(sound_buffer_applause);
      sound_applause.play();
   }
}

void ParchisGUI::playExplosionSound(){
   if (sound_on){
      //Sound sound_explosion;
      //sound_explosion.setBuffer(sound_buffer_explosion);
      sound_explosion.play();
   }
}

void ParchisGUI::playStarhitSound(){
   if (sound_on){
      //Sound sound_starhit;
      //sound_starhit.setBuffer(sound_buffer_starhit);
      sound_starhit.play();
   }
}

void ParchisGUI::playShockSound(){
   if (sound_on){
      //Sound sound_shock;
      //sound_shock.setBuffer(sound_buffer_shock);
      sound_shock.play();
   }
}

void ParchisGUI::playBooSound(){
   if (sound_on){
      //Sound sound_boo;
      //sound_boo.setBuffer(sound_buffer_boo);
      sound_boo.play();
   }
}

void ParchisGUI::playHornSound(){
   if (sound_on){
      //Sound sound_horn;
      //sound_horn.setBuffer(sound_buffer_horn);
      sound_horn.play();
   }
}

void ParchisGUI::playBulletSound(){
   if (sound_on){
      //Sound sound_bullet;
      //sound_bullet.setBuffer(sound_buffer_bullet);
      sound_bullet.play();
   }
}

// Deprecated.
void ParchisGUI::checkHurryUp(){
   if (true){
      //current_background_theme == &background_theme){
      bool hurry_up = false;
      bool win = false;
      vector<color> colors = Parchis::game_colors;

      for (int i = 0; i < colors.size() && !win && !hurry_up; i++){
         if (model->piecesAtGoal(colors[i]) == 3){
            win = true;
         }
         else if (model->piecesAtGoal(colors[i]) == 2){
            hurry_up = true;
         }
      }
      if (win){
         switchBackgroundMusic(background_theme_win);
      }
      else if (hurry_up){
         switchBackgroundMusic(background_theme_hurryup);
      }
   }
}


void ParchisGUI::checkSwitchMusic(){
   bool hurry_up = false;
   bool star = false;
   bool mega = false;
   bool shock = false;

   vector<color> colors = Parchis::game_colors;

   for (int i = 0; i < colors.size(); i++){
      for (int j = 0; j < model->getBoard().getPieces(colors[i]).size(); j++){
         if (model->getBoard().getPieces(colors[i])[j].get_type() == star_piece){
            star = true;
            break;
         }
         else if (model->getBoard().getPieces(colors[i])[j].get_type() == mega_piece){
            mega = true;
            break;
         }
         else if (model->getBoard().getPieces(colors[i])[j].get_type() == small_piece){
            shock = true;
            break;
         }
      }
      if (!star && !mega && !shock && model->piecesAtGoal(colors[i]) == 2){
         hurry_up = true;
      }
   }
   if (star){
      switchBackgroundMusic(background_theme_star);
   }
   else if (mega){
      switchBackgroundMusic(background_theme_mega);
   }
   else if (shock){
      switchBackgroundMusic(background_theme_shock);
   }
   else if (hurry_up){
      switchBackgroundMusic(background_theme_hurryup);
   }
   else{
      switchBackgroundMusic(background_theme);
   }
}

/**
 * genera las posiciones para las casillas de los corredores de llegada a meta
 * @param map mapa para almacenar las posiciones
 * @param col color de la casilla
 * @param x posicion x inicial
 * @param dx variacion en eje X
 * @param y posicion y inicial
 * @param dy variacion en eje Y
 * @param horizontal
 */
void ParchisGUI::generateFinalCorridorPositions(map<Box, vector<Vector2i>>& map, color col, int x, int dx,
                                                int y, int dy, bool horizontal){
   int posx = x, posy = y;
   for (int i = 0; i < 8; i++){
      if (horizontal){
         posx = x + (dx * i);
         map[{i + 1, final_queue, col}] =
            {Vector2i(posx, posy), Vector2i(posx, posy - dy), Vector2i(posx, posy + dy)};
      }
      else{
         posy = y + (dy * i);
         map[{i + 1, final_queue, col}] =
            {Vector2i(posx, posy), Vector2i(posx - dx, posy), Vector2i(posx + dx, posy)};
      }
   }
}

/**
 * genera todos los pasillos de llegada
 * @param map
 */
void ParchisGUI::generateFinalCorridorsPositions(map<Box, vector<Vector2i>>& map){
   // genera las casillas del pasillo amarillo
   generateFinalCorridorPositions(map, color::yellow, 708, -38, 386, 15, true);

   // genera las casillas del pasillo azul
   generateFinalCorridorPositions(map, color::blue, 386, 16, 63, 38, false);

   // genera las casillas del pasillo rojo
   generateFinalCorridorPositions(map, color::red, 63, 38, 386, 15, true);

   // genera las casillas del pasillo verde
   generateFinalCorridorPositions(map, color::green, 386, 16, 708, -38, false);
}

/**
 * genera las casillas seguras con ids 17, 34, 51 y 68
 * @param map mapa para almacenar las posiciones para las casillas
 */
void ParchisGUI::generateSafePositions(map<Box, vector<Vector2i>>& map){
   int start = 25;
   int middle = 386;
   int end = 746;
   int lateralOffset = 15;
   // casilla 17: posicion central para X y posicion inicial para Y (sentido horizontal)
   map[{17, box_type::normal, color::none}] = {
      Vector2i(middle, start),
      Vector2i(middle - lateralOffset, start),
      Vector2i(middle + lateralOffset, start)
   };

   // casilla 34: posicion inicial para X y posicion media para Y (sentido vertical)
   map[{34, box_type::normal, color::none}] = {
      Vector2i(start, middle),
      Vector2i(start, middle - lateralOffset),
      Vector2i(start, middle + lateralOffset)
   };

   // casilla 51: posicion media para X y posicion final para Y (sentido horizontal)
   map[{51, box_type::normal, color::none}] = {
      Vector2i(middle, end),
      Vector2i(middle - lateralOffset, end),
      Vector2i(middle + lateralOffset, end)
   };

   // casilla 68: posicion final para X y posicion media para Y (sentido vertical)
   map[{68, box_type::normal, color::none}] = {
      Vector2i(end, middle),
      Vector2i(end, middle - lateralOffset),
      Vector2i(end, middle + lateralOffset)
   };
}

/**
 * genera las pisiciones de las fichas en casa
 * @param map
 */
void ParchisGUI::generateHomePositions(map<Box, vector<Vector2i>>& map){
   // posiciones para el color amarillo
   map[{0, box_type::home, color::yellow}] = {
      Vector2i(635, 100), Vector2i(670, 135), Vector2i(635, 170), Vector2i(600, 135)
   };

   // posiciones para el color azul
   map[{0, box_type::home, color::blue}] = {
      Vector2i(135, 100), Vector2i(170, 135), Vector2i(135, 170), Vector2i(100, 135)
   };

   // posiciones para el color rojo
   map[{0, box_type::home, color::red}] = {
      Vector2i(135, 600), Vector2i(170, 635), Vector2i(135, 670), Vector2i(100, 635)
   };

   // posiciones para el color verde
   map[{0, box_type::home, color::green}] = {
      Vector2i(635, 600), Vector2i(670, 635), Vector2i(635, 670), Vector2i(600, 635)
   };
}

/**
 * genera las posiciones en la meta
 * @param map
 * @param col
 */
void ParchisGUI::generateGoalPositions(map<Box, vector<Vector2i>>& map){
   // posiciones para el color amarillo
   map[{0, box_type::goal, color::yellow}] = {
      Vector2i(410, 385), Vector2i(445, 350), Vector2i(445, 385), Vector2i(445, 420)
   };

   // posiciones para el color azul
   map[{0, box_type::goal, color::blue}] = {
      Vector2i(385, 360), Vector2i(350, 325), Vector2i(385, 325), Vector2i(420, 325)
   };

   // posiciones para el color rojo
   map[{0, box_type::goal, color::red}] = {
      Vector2i(360, 385), Vector2i(325, 420), Vector2i(325, 385), Vector2i(325, 350)
   };

   // posiciones para el color verde
   map[{0, box_type::goal, color::green}] = {
      Vector2i(385, 410), Vector2i(350, 445), Vector2i(385, 445), Vector2i(420, 445)
   };
}

/**
 * @param map metodo para generar una secuencia de 8 celdas seguidas con las
 * posiciones que tendrian las fichas (primera posicion: ficha en centro;
 * segunda posicion: ficha a la izquierda; tercera posicion: ficha a la derecha)
 * @param boxId identificador (numero) de la primera celda
 * @param lastBoxId identificador (numero) de la celda especial (mas pequeña)
 * @param x posicion X de la primera celda
 * @param dx distancia en X entre celda y celda
 * @param y posicion Y de la primera celda
 * @param dy distancia en Y entre celda y celda
 * @param dlast distancia especial para las celdas mas pequeñas
 * @param horizontal indica si la secuencia se desarrolla en horizontal
 */
void ParchisGUI::generateNormalPositions(map<Box, vector<Vector2i>>& map, int boxId, int lastBoxId,
                                         int x, int dx, int y, int dy, int dlast, bool horizontal){
   int xCenter, xLeft, xRight, yCenter, yLeft, yRight, currentId;
   // considera cada casilla
   for (int i = 0; i < 8; i++){
      // para direccion horizontal
      currentId = boxId + i;
      if (horizontal){
         xCenter = x + (i * dx);
         xLeft = xRight = xCenter;
         yCenter = y;
         yLeft = y - dy;
         yRight = y + dy;

         // modifica las posiciones para la celda de menor dimension
         if (currentId == lastBoxId){
            if (dx < 0){
               // los numeros van de derecha a izquierda: en las posiciones
               // izquierda y derecha se incrementa el valor de Y
               yLeft += dlast;
               yRight += dlast;
            }
            else{
               // los numeros van de izquierda a derecha: en las posiciones
               // izquierda y derecha se decrementa el valor de y
               yLeft -= dlast;
               yRight -= dlast;
            }
         }
      }
      else{
         // sentido vertical
         xCenter = x;
         xLeft = x - dx;
         xRight = x + dx;
         yCenter = y + (i * dy);
         yLeft = yRight = yCenter;

         // se modifican las posiciones de la celda de menor dimension
         if (currentId == lastBoxId){
            if (dy < 0){
               // numeros avanzan de abajo hacia arriba: se decrementa
               // el valor de X de las posiciones laterales
               xLeft -= dlast;
               xRight -= dlast;
            }
            else{
               // numeros avanzan de arriba hacia abajo: se incrementa
               // el valor de X de las posiciones laterales
               xLeft += dlast;
               xRight += dlast;
            }
         }
      }

      // se genera la entrada para el mapa
      vector<Vector2i> posiciones = {
         Vector2i(xCenter, yCenter),
         Vector2i(xLeft, yLeft),
         Vector2i(xRight, yRight)
      };
      map[{currentId, box_type::normal, color::none}] = posiciones;
   }
}

/**
 * genera todas las celdas normales del tablero
 * @param map
 */
void ParchisGUI::generateNormalPositions(map<Box, vector<Vector2i>>& map){
   // TODO: se podrian crear constantes para el desplazamiento en X (38),
   // desplazamiento lateral para posiciones izquierda y derecha;  y correccion para las casillas
   // mas estrechas (8)
   // desplazamiento normal entre casillas: 38

   // secuencia de la 1 a la 8
   generateNormalPositions(map, 1, 8, 746, -38, 310, 15, 8, true);

   // secuencia de la 9 a la 16
   generateNormalPositions(map, 9, 9, 462, 15, 291, -38, 8, false);

   // secuencia de la 18 a la 25
   generateNormalPositions(map, 18, 25, 310, 15, 25, 38, 8, false);

   // secuencia de la 26 a la 33
   generateNormalPositions(map, 26, 26, 291, -38, 310, 15, 8, true);

   // secuencia de la 35 a la 42
   generateNormalPositions(map, 35, 42, 25, 38, 462, 15, 8, true);

   // secuencia de la 43 a la 50
   generateNormalPositions(map, 43, 43, 310, 15, 480, 38, 8, false);

   // secuencia de la 52 a 59
   generateNormalPositions(map, 52, 59, 462, 15, 746, -38, 8, false);

   // secuencia de 60 a 67
   generateNormalPositions(map, 60, 60, 480, 38, 462, 15, 8, true);
}

/**
 *
 * @return se generan todas las posiciones del tablero
 */
map<Box, vector<Vector2i>> ParchisGUI::generatePositions(){
   // se generan el almacen de celdas
   map<Box, vector<Vector2i>> boxesMap;

   // generacion de las celdas normales
   generateNormalPositions(boxesMap);

   // se generan las posiciones de casa
   generateHomePositions(boxesMap);

   // se generan las fichas de los corredores
   generateFinalCorridorsPositions(boxesMap);

   // se generan las celdas seguras
   generateSafePositions(boxesMap);

   // se generan los corredores de llegada a la meta
   generateFinalCorridorsPositions(boxesMap);

   // se generan las posiciones de meta
   generateGoalPositions(boxesMap);

   // se devuelve el mapa
   return boxesMap;
}
