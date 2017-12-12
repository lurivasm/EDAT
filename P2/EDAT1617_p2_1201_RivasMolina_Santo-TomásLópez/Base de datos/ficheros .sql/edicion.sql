-- Table: public.edicion

-- DROP TABLE public.edicion;

CREATE TABLE public.edicion
(
  isbn text NOT NULL,
  fecha_publicacion date,
  pags text,
  titulo text NOT NULL,
  idioma text,
  formato text,
  precio money NOT NULL,
  editorial text,
  autor text NOT NULL,
  CONSTRAINT edicion_pkey PRIMARY KEY (isbn, titulo, autor)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.edicion
  OWNER TO alumnodb;
