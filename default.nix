with (import <nixpkgs> {}).pkgs;

callPackage (

{ stdenv, SDL2, SDL2_image }:

stdenv.mkDerivation {
  name = "c-maze-0.0.0.0";
  src = ./.;
  buildInputs = [ SDL2 SDL2_image ];
}

) {}
