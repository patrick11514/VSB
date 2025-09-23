import { BootstrapIcon } from '../types/bootstrap_icons';

export default function Icon({
    name,
    className
}: {
    name: BootstrapIcon;
    className?: string;
}) {
    return <i className={`bi ${name} ${className ?? ''}`} />;
}
