#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <memory>

class TetrisGame {
private:
    static constexpr int GRID_HEIGHT = 20;
    static constexpr int GRID_WIDTH = 10;
    static constexpr float INITIAL_DELAY = 0.3f;
    static constexpr float FAST_DELAY = 0.05f;
    static constexpr int TILE_SIZE = 18;
    static constexpr int OFFSET_X = 28;
    static constexpr int OFFSET_Y = 31;

    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
    };

    class Tetromino {
    public:
        static const std::array<std::array<int, 4>, 7> SHAPES;
        std::array<Point, 4> blocks;
        int colorNum;

        Tetromino(int shapeNum, int color) : colorNum(color) {
            for (int i = 0; i < 4; ++i) {
                blocks[i].x = SHAPES[shapeNum][i] % 2;
                blocks[i].y = SHAPES[shapeNum][i] / 2;
            }
        }

        void rotate(const Point& center) {
            for (auto& block : blocks) {
                int x = block.y - center.y;
                int y = block.x - center.x;
                block.x = center.x - x;
                block.y = center.y + y;
            }
        }

        void move(int dx, int dy) {
            for (auto& block : blocks) {
                block.x += dx;
                block.y += dy;
            }
        }
    };

    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> grid{};
    std::unique_ptr<Tetromino> currentPiece;
    std::mt19937 rng;
    sf::RenderWindow window;
    sf::Texture tileTexture, bgTexture, frameTexture;
    sf::Sprite tileSprite, bgSprite, frameSprite;
    float timer{0.0f};
    float currentDelay{INITIAL_DELAY};

    bool isValidPosition(const Tetromino& piece) const {
        for (const auto& block : piece.blocks) {
            if (block.x < 0 || block.x >= GRID_WIDTH || block.y >= GRID_HEIGHT) {
                return false;
            }
            if (block.y >= 0 && grid[block.y][block.x] != 0) {
                return false;
            }
        }
        return true;
    }

    void createNewPiece() {
        std::uniform_int_distribution<int> shapeDist(0, 6);
        std::uniform_int_distribution<int> colorDist(1, 7);
        currentPiece = std::make_unique<Tetromino>(shapeDist(rng), colorDist(rng));
    }

    void lockPiece() {
        for (const auto& block : currentPiece->blocks) {
            if (block.y >= 0) {
                grid[block.y][block.x] = currentPiece->colorNum;
            }
        }
        createNewPiece();
    }

    void clearLines() {
        int writeRow = GRID_HEIGHT - 1;
        for (int readRow = GRID_HEIGHT - 1; readRow >= 0; --readRow) {
            int blockCount = 0;
            for (int col = 0; col < GRID_WIDTH; ++col) {
                if (grid[readRow][col] != 0) {
                    ++blockCount;
                }
            }
            
            if (blockCount < GRID_WIDTH) {
                if (writeRow != readRow) {
                    for (int col = 0; col < GRID_WIDTH; ++col) {
                        grid[writeRow][col] = grid[readRow][col];
                    }
                }
                --writeRow;
            }
        }

        // Clear remaining rows at top
        for (int row = writeRow; row >= 0; --row) {
            for (int col = 0; col < GRID_WIDTH; ++col) {
                grid[row][col] = 0;
            }
        }
    }

    void drawBlock(const Point& pos, int color) {
        tileSprite.setTextureRect(sf::IntRect(color * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
        tileSprite.setPosition(
            pos.x * TILE_SIZE + OFFSET_X,
            pos.y * TILE_SIZE + OFFSET_Y
        );
        window.draw(tileSprite);
    }

public:
    TetrisGame() : 
        rng(std::random_device{}()),
        window(sf::VideoMode(320, 480), "Tetris") 
    {
        if (!tileTexture.loadFromFile("images/tiles.png") ||
            !bgTexture.loadFromFile("images/background.png") ||
            !frameTexture.loadFromFile("images/frame.png")) {
            throw std::runtime_error("Failed to load textures");
        }

        tileSprite.setTexture(tileTexture);
        bgSprite.setTexture(bgTexture);
        frameSprite.setTexture(frameTexture);
        
        createNewPiece();
    }

    void run() {
        sf::Clock clock;

        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            timer += deltaTime;

            handleInput();
            update();
            render();
        }
    }

private:
    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                auto tempPiece = *currentPiece;
                
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        tempPiece.rotate(tempPiece.blocks[1]);
                        if (isValidPosition(tempPiece)) {
                            *currentPiece = tempPiece;
                        }
                        break;
                    case sf::Keyboard::Left:
                        tempPiece.move(-1, 0);
                        if (isValidPosition(tempPiece)) {
                            *currentPiece = tempPiece;
                        }
                        break;
                    case sf::Keyboard::Right:
                        tempPiece.move(1, 0);
                        if (isValidPosition(tempPiece)) {
                            *currentPiece = tempPiece;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        currentDelay = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? 
                      FAST_DELAY : INITIAL_DELAY;
    }

    void update() {
        if (timer > currentDelay) {
            timer = 0;
            
            auto tempPiece = *currentPiece;
            tempPiece.move(0, 1);
            
            if (isValidPosition(tempPiece)) {
                *currentPiece = tempPiece;
            } else {
                lockPiece();
                clearLines();
            }
        }
    }

    void render() {
        window.clear(sf::Color::White);
        window.draw(bgSprite);

        // Draw grid
        for (int row = 0; row < GRID_HEIGHT; ++row) {
            for (int col = 0; col < GRID_WIDTH; ++col) {
                if (grid[row][col] != 0) {
                    drawBlock({col, row}, grid[row][col]);
                }
            }
        }

        // Draw current piece
        for (const auto& block : currentPiece->blocks) {
            drawBlock(block, currentPiece->colorNum);
        }

        window.draw(frameSprite);
        window.display();
    }
};

// Initialize static member
const std::array<std::array<int, 4>, 7> TetrisGame::Tetromino::SHAPES = {{
    {1,3,5,7}, // I
    {2,4,5,7}, // Z
    {3,5,4,6}, // S
    {3,5,4,7}, // T
    {2,3,5,7}, // L
    {3,5,7,6}, // J
    {2,3,4,5}  // O
}};

int main() {
    try {
        TetrisGame game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}