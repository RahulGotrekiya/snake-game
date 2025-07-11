{
  description = "A classic Snake game implemented in C using ncurses";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages.default = pkgs.callPackage ./default.nix { };
        
        packages.snake-game = self.packages.${system}.default;

        apps.default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/snake";
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            make
            pkg-config
            ncurses
            ncurses.dev
            
            # Development tools
            gdb
            valgrind
          ];
          
          shellHook = ''
            echo "Snake Game Development Environment"
            echo "================================="
            echo "Available commands:"
            echo "  make           - Build the game"
            echo "  make run       - Build and run the game"
            echo "  nix build      - Build with Nix"
            echo "  nix run        - Run the game directly"
            echo ""
          '';
        };
      });
}