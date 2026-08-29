export function Icon({ name, fill = false, className = "", style }) {
  return (
    <span
      className={`material-symbols-outlined${fill ? " fill" : ""}${className ? " " + className : ""}`}
      style={style}
    >
      {name}
    </span>
  );
}
