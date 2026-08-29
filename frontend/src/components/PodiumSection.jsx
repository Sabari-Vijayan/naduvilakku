import { podiumPlayers } from "../data/leaderboard";

// The podium order in the HTML is: rank2 (left), rank1 (center), rank3 (right)
// But rank3 has the tallest bar (h-44) since it shows position 1 in the party ranking
// We keep the same visual layout as the original design
export function PodiumSection() {
  return (
    <section className="flex justify-center items-end gap-podium-gap h-64 mt-8">
      {podiumPlayers.map((player) => (
        <div key={player.rank} className="flex flex-col items-center flex-1">
          <img
            src={player.avatar}
            alt={player.name}
            className={`${player.avatarSize} rounded-full object-cover ${player.borderWidth} ${player.borderColor} mb-2 ${player.shadow} z-10`}
          />
          <div className="text-center mb-2">
            <p className={player.nameStyle}>{player.name}</p>
            <p className={player.pointsStyle}>{player.points.toLocaleString()} points</p>
          </div>
          <div
            className={`w-full bg-tertiary-fixed ${player.podiumHeight} rounded-t-xl flex ${player.rankPosition} shadow-inner`}
          >
            <span className={player.rankStyle}>{player.rank}</span>
          </div>
        </div>
      ))}
    </section>
  );
}
