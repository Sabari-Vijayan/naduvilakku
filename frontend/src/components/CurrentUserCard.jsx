import { Icon } from "./Icon";
import { currentUser } from "../data/leaderboard";

export function CurrentUserCard() {
  return (
    <section className="bg-primary-container rounded-xl p-6 shadow-lg relative overflow-hidden flex items-center gap-4 text-on-primary-container">
      <img
        src={currentUser.avatar}
        alt={currentUser.name}
        className="w-14 h-14 rounded-full border-2 border-on-primary-container object-cover"
      />
      <div className="flex-grow flex justify-between items-center">
        <div>
          <p className="font-body-sm text-body-sm opacity-80">Points:</p>
          <p className="font-headline-md text-headline-md">{currentUser.points}</p>
        </div>
        <div>
          <p className="font-body-sm text-body-sm opacity-80">Level:</p>
          <p className="font-headline-md text-headline-md flex items-center gap-1">
            {currentUser.level}{" "}
            <Icon name="star" fill className="text-sm" />
          </p>
        </div>
        <div>
          <p className="font-body-sm text-body-sm opacity-80">Position:</p>
          <p className="font-headline-md text-headline-md">{currentUser.position}</p>
        </div>
      </div>
    </section>
  );
}
