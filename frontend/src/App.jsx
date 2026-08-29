import { TopAppBar } from "./components/TopAppBar";
import { BottomNavBar } from "./components/BottomNavBar";
import { PodiumSection } from "./components/PodiumSection";
import { CurrentUserCard } from "./components/CurrentUserCard";
import { RankingList } from "./components/RankingList";

export default function App() {
  return (
    <div className="bg-background text-on-background min-h-screen flex flex-col font-body-lg">
      <TopAppBar />

      <main className="flex-grow px-margin-page pb-24 pt-stack-md flex flex-col gap-stack-lg max-w-md mx-auto w-full">
        <PodiumSection />
        <CurrentUserCard />
        <RankingList />
      </main>

      <BottomNavBar />
    </div>
  );
}
