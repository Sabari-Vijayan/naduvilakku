import { Icon } from "./Icon";

const navItems = [
  { icon: "home", label: "Home", active: false },
  { icon: "leaderboard", label: "Leaderboard", active: true },
  { icon: "emoji_events", label: "Events", active: false },
  { icon: "person", label: "Profile", active: false },
];

export function BottomNavBar() {
  return (
    <nav className="fixed bottom-0 left-0 w-full z-50 flex justify-around items-center px-6 py-3 bg-surface shadow-lg rounded-t-xl shadow-[0_-4px_20px_rgba(0,0,0,0.05)] md:hidden">
      {navItems.map((item) =>
        item.active ? (
          <a
            key={item.label}
            href="#"
            className="flex flex-col items-center justify-center bg-primary-container text-on-primary-container rounded-full p-3 active:scale-90 transition-transform"
          >
            <Icon name={item.icon} fill />
            <span className="font-label-caps text-label-caps mt-1 sr-only">
              {item.label}
            </span>
          </a>
        ) : (
          <a
            key={item.label}
            href="#"
            className="flex flex-col items-center justify-center text-on-surface-variant p-3 hover:text-primary transition-colors group"
          >
            <Icon name={item.icon} className="group-hover:scale-110 transition-transform" />
            <span className="font-label-caps text-label-caps mt-1 sr-only">
              {item.label}
            </span>
          </a>
        )
      )}
    </nav>
  );
}
