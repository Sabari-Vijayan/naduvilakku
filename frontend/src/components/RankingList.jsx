import { Icon } from "./Icon";
import { rankingList } from "../data/leaderboard";

export function RankingList() {
  return (
    <section className="flex flex-col gap-gutter-row">
      {rankingList.map((player) => (
        <div
          key={player.rank}
          className="flex items-center gap-4 bg-surface-container-lowest p-3 rounded-lg border border-surface-variant hover:bg-surface-container-low transition-colors"
        >
          <span className="font-rank-number text-rank-number text-on-surface-variant w-8 text-right">
            {String(player.rank).padStart(2, "0")}
          </span>
          <div className="flex-grow">
            <p className="font-body-lg text-body-lg text-on-surface">{player.name}</p>
            <p className="font-body-sm text-body-sm text-on-surface-variant">
              {player.points.toLocaleString()} points
            </p>
          </div>
          <Icon
            name="military_tech"
            fill
            className=""
            style={{ color: player.medalColor }}
          />
        </div>
      ))}
    </section>
  );
}
