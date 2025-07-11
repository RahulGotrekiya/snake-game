{ lib
, stdenv
, fetchFromGitHub
, ncurses
, pkg-config
}:

stdenv.mkDerivation rec {
  pname = "snake-game";
  version = "1.0.0";

  src = ./.;

  nativeBuildInputs = [
    pkg-config
  ];

  buildInputs = [
    ncurses
  ];

  makeFlags = [
    "PREFIX=$(out)"
  ];

  installPhase = ''
    runHook preInstall
    
    mkdir -p $out/bin
    cp bin/snake $out/bin/
    
    runHook postInstall
  '';

  meta = with lib; {
    description = "A classic Snake game implemented in C using ncurses";
    longDescription = ''
      A terminal-based Snake game with colorful graphics, smooth gameplay,
      and classic snake mechanics. Features include score tracking, collision
      detection, and responsive controls using WASD or arrow keys.
    '';
    homepage = "https://github.com/RahulGotrekiya/snake-game";
    license = licenses.mit;
    maintainers = with maintainers; [ RahulGotrekiya ];
    platforms = platforms.unix;
    mainProgram = "snake";
  };
}