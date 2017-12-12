-- Table: public.oferta

-- DROP TABLE public.oferta;

CREATE TABLE public.oferta
(
  num_oferta integer NOT NULL,
  descuento integer,
  fecha_inicio date,
  fecha_fin date,
  edicion text NOT NULL,
  CONSTRAINT oferta_pkey PRIMARY KEY (num_oferta, edicion)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.oferta
  OWNER TO alumnodb;
