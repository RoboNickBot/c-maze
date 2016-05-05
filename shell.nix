with (import <nixpkgs> {}).pkgs;

let der = { stdenv, SDL2, SDL2_image }: stdenv.mkDerivation rec {
  name = "c-maze-0.1";
  builder = ./builder.sh;
  src = ./.;
  inherit SDL2 SDL2_image;
};

in

(callPackage der {})
