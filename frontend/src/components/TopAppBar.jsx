import { Icon } from "./Icon";

export function TopAppBar() {
  return (
    <header className="bg-surface sticky top-0 z-40 flex justify-between items-center px-margin-page py-4 w-full">
      <button
        aria-label="Back"
        className="text-primary hover:bg-surface-variant transition-colors p-2 rounded-full"
      >
        <Icon name="arrow_back" />
      </button>
      <h1 className="text-headline-lg-mobile font-headline-lg-mobile text-primary">
        Best Party
      </h1>
      <button
        aria-label="More options"
        className="text-primary hover:bg-surface-variant transition-colors p-2 rounded-full"
      >
        <Icon name="more_vert" />
      </button>
    </header>
  );
}
