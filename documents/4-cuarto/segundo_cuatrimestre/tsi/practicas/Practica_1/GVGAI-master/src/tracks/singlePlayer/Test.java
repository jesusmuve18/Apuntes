package tracks.singlePlayer;

import java.util.Random;

import core.logging.Logger;
import tools.Utils;
import tracks.ArcadeMachine;


/** 
 * Created with IntelliJ IDEA. User: Diego Date: 04/10/13 Time: 16:29 This is a
 * Java port from Tom Schaul's VGDL - https://github.com/schaul/py-vgdl
 */
public class Test {

	public static void probarTodo(String game, String gameName, int seed) {
		// Definimos los controladores
		String[] controladores = {
			"tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteProfundidad",
			"tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteAStar",
			"tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteRTAStar",
			"tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteLRTAStarK"
		};

		String[] nombresCorta = {"Profundidad", 
								 "AStar      ", 
								 "RTAStar    ", 
								 "LRTAStarK  "};

		// Bucle de controladores
		for (int i = 0; i < controladores.length; i++) {			
			// Bucle de niveles (del 0 al 7)
			for (int lvl = 0; lvl <= 8; lvl++) {
				String levelPath = game.replace(gameName, gameName + "_lvl" + lvl);
				String recordFile = "output/result_" + nombresCorta[i].trim() +"_lvl_" + lvl + ".txt";
				
				System.out.println("");
				System.out.println("***********************************");
				System.out.println("*  Agente: " + nombresCorta[i] + "  Nivel: " + lvl + "  *");
				System.out.println("***********************************");
				
				MetricsProvider.getInstance().reset();	// Reinicio las métricas
				// Ejecutamos el juego. 
				// 'visuals = false' para que vaya a máxima velocidad, 
				ArcadeMachine.runOneGame(game, levelPath, false, controladores[i], recordFile, seed, 0);
			}
		}
	}


    public static void main(String[] args) {

		// Available tracks:
		String sampleRandomController = "tracks.singlePlayer.simple.sampleRandom.Agent";
		String doNothingController = "tracks.singlePlayer.simple.doNothing.Agent";
		String sampleOneStepController = "tracks.singlePlayer.simple.sampleonesteplookahead.Agent";
		String sampleFlatMCTSController = "tracks.singlePlayer.simple.greedyTreeSearch.Agent";

		String sampleMCTSController = "tracks.singlePlayer.advanced.sampleMCTS.Agent";
        String sampleRSController = "tracks.singlePlayer.advanced.sampleRS.Agent";
        String sampleRHEAController = "tracks.singlePlayer.advanced.sampleRHEA.Agent";
		String sampleOLETSController = "tracks.singlePlayer.advanced.olets.Agent";

		//Load available games
		String spGamesCollection =  "examples/all_games_sp.csv";
		String[][] games = Utils.readGames(spGamesCollection);

		//Game settings
		boolean visuals = true;
		int seed = new Random().nextInt();

		// Game and level to play
		int gameIdx = 122;
		int levelIdx = 6; // level names from 0 to 4 (game_lvlN.txt).
		String gameName = games[gameIdx][1];
		String game = games[gameIdx][0];
		String level1 = game.replace(gameName, gameName + "_lvl" + levelIdx);

		// String recordActionsFile =  "actions_" + games[gameIdx] + "_lvl"
		// 				+ levelIdx + "_" + seed + ".txt";
						// where to record the actions
						// executed. null if not to save.

		// 1. This starts a game, in a level, played by a human.
		// ArcadeMachine.playOneGame(game, level1, null, seed);

		// 2. This plays a game in a level by the controller.

		String Controller = "";
		
		// 1. Búsqueda en profundidad
		// 2. A*
		// 3. RTA*
		// 4. LRTA*(k)
		int controllerId = 2;

		switch (controllerId) {
			case 1:
				Controller = "tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteProfundidad";
				break;
			case 2:
				Controller = "tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteAStar";
				break;
			case 3:
				Controller = "tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteRTAStar";
				break;
			case 4:
				Controller = "tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS.AgenteLRTAStarK";
				break;
		}

		// String recordActionsFile =  "actions_catapults2026_lvl"
		// 				+ levelIdx + "_alg_" + controllerId + ".txt";

		// ArcadeMachine.runOneGame(game, level1, visuals, Controller, null, seed, 0);

		probarTodo(game, gameName, seed);


	// 3. This replays a game from an action file previously recorded
	//	 String readActionsFile = recordActionsFile;
	//	 ArcadeMachine.replayGame(game, level1, visuals, readActionsFile);

		// 4. This plays a single game, in N levels, M times :
//		String level2 = new String(game).replace(gameName, gameName + "_lvl" + 1);
//		int M = 10;
//		for(int i=0; i<games.length; i++){
//			game = games[i][0];
//			gameName = games[i][1];
//			level1 = game.replace(gameName, gameName + "_lvl" + levelIdx);
//			ArcadeMachine.runGames(game, new String[]{level1}, M, sampleMCTSController, null);
//		}

		//5. This plays N games, in the first L levels, M times each. Actions to file optional (set saveActions to true).
//		int N = games.length, L = 2, M = 1;
//		boolean saveActions = false;
//		String[] levels = new String[L];
//		String[] actionFiles = new String[L*M];
//		for(int i = 0; i < N; ++i)
//		{
//			int actionIdx = 0;
//			game = games[i][0];
//			gameName = games[i][1];
//			for(int j = 0; j < L; ++j){
//				levels[j] = game.replace(gameName, gameName + "_lvl" + j);
//				if(saveActions) for(int k = 0; k < M; ++k)
//				actionFiles[actionIdx++] = "actions_game_" + i + "_level_" + j + "_" + k + ".txt";
//			}
//			ArcadeMachine.runGames(game, levels, M, sampleRHEAController, saveActions? actionFiles:null);
//		}
    }
}
