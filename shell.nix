{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Build tools
    gcc
    gnumake
    pkg-config
    
    # Libraries
    ncurses
    ncurses.dev
    
    # Development tools (optional)
    gdb
    valgrind
  ];
  
  shellHook = ''
    echo "Snake Game Development Environment"
    echo "================================="
    echo "Available commands:"
    echo "  make           - Build the game"
    echo "  make run       - Build and run the game"
    echo "  make clean     - Clean build artifacts"
    echo "  make debug     - Build with debug symbols"
    echo "  make check-deps - Check dependencies"
    echo ""
    echo "Game Controls:"
    echo "  WASD or Arrow Keys - Move snake"
    echo "  Q - Quit game"
    echo "  R - Restart (when game over)"
    echo ""
    
    # Set up environment variables
    export PKG_CONFIG_PATH="${pkgs.ncurses.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"
  '';
}